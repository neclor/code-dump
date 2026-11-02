

model = Model(HiGHS.Optimizer) #



# ======= Land ======= #

@variable(model, crop_areas[block_names, crop_names] >= 0)

# Area of each crop on the whole farm.
@expression(model, crop_total_area[crop in crop_names], sum(crop_areas[:, crop]))

# Sum of the areas sown on a block must equal to its area.
@constraint(model, land_c[block in block_names], sum(crop_areas[block, :]) == block_data[block].area_ha)

# 7. Diversification. No crop may occupy more than its max_farm_share of the farm area in a year.
@constraint(model, diversification_c[crop in crop_names], crop_total_area[crop] <= crop_data[crop].max_farm_share * farm_area_ha)

# 8. Non-productive area. Minimum share of the farm left non-productive.
@expression(model, non_productive_area, sum(crop_total_area[crop] for crop in crop_names if crop_data[crop].is_non_productive))
@constraint(model, non_productive_minimum_c, non_productive_area >= rules_data.non_productive_share * farm_area_ha)

# Crop cost.
@expression(model, crop_cost, sum(crop_total_area[crop] * crop_data[crop].variable_cost_EUR_per_ha for crop in crop_names))

# Crop production.
@expression(
    model,
    crop_production[crop in crop_names],
    sum(
        crop_areas[block, crop] * crop_data[crop].yields[block_data[block].soil_class]
        for block in block_names
    )
)

# ======= Market ======= #

# Sales.
crops_for_sale = [
    crop for crop in crop_names
    if crop_data[crop].market !== nothing
]

@variable(model, crop_sales[crops_for_sale] >= 0)

crops_with_contract_min = [crop for crop in crops_for_sale if crop_data[crop].market.contract_min_t !== nothing]
crops_with_contract_max = [crop for crop in crops_for_sale if crop_data[crop].market.contract_max_t !== nothing]

@constraint(model, contract_min_c[crop in crops_with_contract_min], crop_sales[crop] >= crop_data[crop].market.contract_min_t)
@constraint(model, contract_max_c[crop in crops_with_contract_max], crop_sales[crop] <= crop_data[crop].market.contract_max_t)

@constraint(model, sales_limit_c[crop in crops_for_sale], crop_sales[crop] <= crop_production[crop])

# Spot sales.
crops_for_spot_sale = [
    crop for crop in crops_for_sale
    if crop_data[crop].market.spot_price_EUR_per_t !== nothing
]

@expression(model, crop_spot_sales[crop in crops_for_spot_sale], crop_production[crop] - crop_sales[crop])

# Revenue
@expression(
    model,
    revenue,
    sum(crop_sales[crop] * crop_data[crop].market.price_EUR_per_t for crop in crops_for_sale)
    + sum(crop_spot_sales[crop] * crop_data[crop].market.spot_price_EUR_per_t for crop in crops_for_spot_sale)
)

# ======= Water ======= #

@variable(model, 0 <= level[periods] <= water_data.reservoir_capacity_m3)
@variable(model, 0 <= pumped[period in periods] <= water_data.periods[period].pumping_cap_m3)
@variable(model, spilled[periods] >= 0)

# Watering the crops.
@expression(
    model,
    irrigation[period in periods],
    sum(
        crop_total_area[crop] * crop_data[crop].calendar[period].irrigation_m3_per_ha
        for crop in crop_names
    )
)

# Abstraction permit a maximum volume per year.
@constraint(model, abstraction_permit_c, sum(pumped) <= water_data.abstraction_licence_m3)

# Reservoir balance of each period, reservoir_balance_c[period].
reservoir_balance_c = Dict{Period, ConstraintRef}()
for (i, p) in enumerate(periods)
    previous_level = i == 1 ? water_data.reservoir_initial_m3 : level[periods[i - 1]]
    reservoir_balance_c[p] = @constraint(
        model,
        level[p] == previous_level + water_data.periods[p].reservoir_inflow_m3 + pumped[p] - irrigation[p] - spilled[p],
        base_name = "reservoir_balance_c[$p]"
    )
end

# The reservoir level at the end of the last period.
@constraint(model, reservoir_terminal_c, level[periods[end]] >= water_data.reservoir_terminal_m3)

# Pumping cost.
@expression(model, pumping_cost, sum(pumped) * water_data.pumping_cost_EUR_per_m3)

# ======= Labour ======= #

# Hired.
@variable(model, hired[periods] >= 0)
@constraint(model, hired_cap_c[period in periods], hired[period] <= labour_data[period].hired_hours_cap)

# Labour demand.
@expression(
    model,
    labour_demand[period in periods],
    sum(
        crop_total_area[crop] * crop_data[crop].calendar[period].labour_h_per_ha
        for crop in crop_names
    )
)

@constraint(model, labour_limit_c[period in periods], labour_demand[period] <= labour_data[period].family_hours + hired[period])

# Hire cost.
@expression(model, hire_cost, sum(hired[period] * labour_data[period].hired_cost_EUR_per_h for period in periods))

# ======= Objective ======= #

@expression(model, profit, revenue - crop_cost - pumping_cost - hire_cost)

@objective(model, Max, profit)

optimize!(model)

if termination_status(model) != MOI.OPTIMAL
    error("No optimal solution found: $(termination_status(model))")
end
