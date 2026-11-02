using JuMP, Gurobi

include("data.jl")

model = Model(Gurobi.Optimizer)

years = 1:rules_data.horizon_years

use_return_period = true
use_preceding_crop = true
use_legume = true

# ======= Land ======= #

@variable(model, crop_areas[years, block_names, crop_names] >= 0)

@expression(model, crop_total_area[year in years, crop in crop_names], sum(crop_areas[year, :, crop]))

@constraint(model, land_c[year in years, block in block_names], sum(crop_areas[year, block, :]) == block_data[block].area_ha)

# 7. Diversification. No crop may occupy more than its max_farm_share of the farm area in a year.
@constraint(
    model,
    diversification_c[year in years, crop in crop_names],
    crop_total_area[year, crop] <= crop_data[crop].max_farm_share * farm_area_ha
)

# 8. Non-productive area. Minimum share of the farm left non-productive.
@expression(
    model,
    non_productive_area[year in years],
    sum(crop_total_area[year, crop] for crop in crop_names if crop_data[crop].is_non_productive)
)
@constraint(model, non_productive_minimum_c[year in years], non_productive_area[year] >= rules_data.non_productive_share * farm_area_ha)

@expression(
    model,
    crop_cost[year in years],
    sum(crop_total_area[year, crop] * crop_data[crop].variable_cost_EUR_per_ha for crop in crop_names)
)

@expression(
    model,
    crop_production[year in years, crop in crop_names],
    sum(
        crop_areas[year, block, crop] * crop_data[crop].yields[block_data[block].soil_class]
        for block in block_names
    )
)

# ======= Market ======= #

# Sales.
crops_for_sale = [
    crop for crop in crop_names
    if crop_data[crop].market !== nothing
]

@variable(model, crop_sales[years, crops_for_sale] >= 0)

crops_with_contract_min = [crop for crop in crops_for_sale if crop_data[crop].market.contract_min_t !== nothing]
crops_with_contract_max = [crop for crop in crops_for_sale if crop_data[crop].market.contract_max_t !== nothing]

@constraint(
    model,
    contract_min_c[year in years, crop in crops_with_contract_min],
    crop_sales[year, crop] >= crop_data[crop].market.contract_min_t
)
@constraint(
    model,
    contract_max_c[year in years, crop in crops_with_contract_max],
    crop_sales[year, crop] <= crop_data[crop].market.contract_max_t
)

@constraint(model, sales_limit_c[year in years, crop in crops_for_sale], crop_sales[year, crop] <= crop_production[year, crop])

# Spot sales.
crops_for_spot_sale = [
    crop for crop in crops_for_sale
    if crop_data[crop].market.spot_price_EUR_per_t !== nothing
]

@expression(
    model,
    crop_spot_sales[year in years, crop in crops_for_spot_sale],
    crop_production[year, crop] - crop_sales[year, crop]
)

# Revenue
@expression(
    model,
    revenue[year in years],
    sum(crop_sales[year, crop] * crop_data[crop].market.price_EUR_per_t for crop in crops_for_sale)
        + sum(crop_spot_sales[year, crop] * crop_data[crop].market.spot_price_EUR_per_t for crop in crops_for_spot_sale)
)

# ======= Water ======= #

@variable(model, 0 <= level[years, periods] <= water_data.reservoir_capacity_m3)
@variable(model, 0 <= pumped[year in years, period in periods] <= water_data.periods[period].pumping_cap_m3)
@variable(model, spilled[years, periods] >= 0)

@expression(
    model,
    irrigation[year in years, period in periods],
    sum(
        crop_total_area[year, crop] * crop_data[crop].calendar[period].irrigation_m3_per_ha
        for crop in crop_names
    )
)

@constraint(model, abstraction_permit_c[year in years], sum(pumped[year, :]) <= water_data.abstraction_licence_m3)

function previous_level(year, period)
    p = Int(period) + 1
    if p > 1
        return level[year, periods[p - 1]]
    elseif year > 1
        return level[year - 1, periods[end]]
    else
        return water_data.reservoir_initial_m3
    end
end

# Reservoir balance of each period of each year.
@constraint(
    model,
    reservoir_balance_c[year in years, period in periods],
    level[year, period] == previous_level(year, period) + water_data.periods[period].reservoir_inflow_m3 + pumped[year, period] - irrigation[year, period] - spilled[year, period]
)

@constraint(model, reservoir_terminal_c, level[years[end], periods[end]] >= water_data.reservoir_terminal_m3)

@expression(model, pumping_cost[year in years], sum(pumped[year, :]) * water_data.pumping_cost_EUR_per_m3)

# ======= Labour ======= #

@variable(model, hired[years, periods] >= 0)
@constraint(model, hired_cap_c[year in years, period in periods], hired[year, period] <= labour_data[period].hired_hours_cap)

@expression(
    model,
    labour_demand[year in years, period in periods],
    sum(
        crop_total_area[year, crop] * crop_data[crop].calendar[period].labour_h_per_ha
        for crop in crop_names
    )
)

@constraint(
    model,
    labour_limit_c[year in years, period in periods],
    labour_demand[year, period] <= labour_data[period].family_hours + hired[year, period]
)

@expression(
    model,
    hire_cost[year in years],
    sum(hired[year, period] * labour_data[period].hired_cost_EUR_per_h for period in periods)
)

# ======= Rotation ======= #

function previous_crop_area(year, block, crop)
    if year == 1
        return block_data[block].initial_areas[crop]
    end
    return crop_areas[year - 1, block, crop]
end

# Return period.
if use_return_period
    @constraint(
        model,
        return_period_c[year in years, block in block_names, crop in crop_names],
        previous_crop_area(year, block, crop) + crop_areas[year, block, crop] <= crop_data[crop].return_share * block_data[block].area_ha
    )
end

# Preceding crop.
if use_preceding_crop
    cereal_crops = [crop for crop in crop_names if crop_data[crop].group == cereal]

    @constraint(
        model,
        preceding_crop_c[year in years, block in block_names],
        crop_areas[year, block, potato] <= sum(previous_crop_area(year, block, crop) for crop in cereal_crops)
    )
end

# Legume.
if use_legume
    legume_crops = [crop for crop in crop_names if crop_data[crop].is_legume]

    @constraint(
        model,
        legume_c[year in years],
        sum(crop_total_area[year, crop] for crop in legume_crops) >= rules_data.legume_share * farm_area_ha
    )
end

# ======= Objective ======= #

@expression(model, profit[year in years], revenue[year] - crop_cost[year] - pumping_cost[year] - hire_cost[year])

discount_factors = [1 / (1 + rules_data.discount_rate)^(year - 1) for year in years]

@expression(model, discounted_profit, sum(discount_factors[year] * profit[year] for year in years))

@objective(model, Max, discounted_profit)

optimize!(model)

if termination_status(model) != MOI.OPTIMAL
    error("No optimal solution found: $(termination_status(model))")
end
