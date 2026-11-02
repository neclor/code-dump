include("c.jl")

println("# ======= C ======= #\n")

println("Gross margin")
println(rpad("year", 8), rpad("revenue", 14), rpad("crop cost", 14), rpad("pumping", 12), rpad("hire", 12), rpad("margin", 14), "weight")
for year in years
    print(rpad(year, 8))
    print(rpad(round(value(revenue[year]), digits = 2), 14))
    print(rpad(round(value(crop_cost[year]), digits = 2), 14))
    print(rpad(round(value(pumping_cost[year]), digits = 2), 12))
    print(rpad(round(value(hire_cost[year]), digits = 2), 12))
    print(rpad(round(value(profit[year]), digits = 2), 14))
    println(round(discount_factors[year], digits = 4))
end
println("Discounted gross margin: ", round(objective_value(model), digits = 2))

println("\nCropping plan, year 0 (ha), the history")
print(rpad("crop", 16))
for block in block_names
    print(rpad(block, 10))
end
println("farm")
for crop in crop_names
    print(rpad(crop, 16))
    for block in block_names
        print(rpad(block_data[block].initial_areas[crop], 10))
    end
    println(sum(block_data[block].initial_areas[crop] for block in block_names))
end

for year in years
    println("\nCropping plan, year ", year, " (ha)")
    print(rpad("crop", 16))
    for block in block_names
        print(rpad(block, 10))
    end
    println("farm")
    for crop in crop_names
        print(rpad(crop, 16))
        for block in block_names
            print(rpad(round(value(crop_areas[year, block, crop]), digits = 2), 10))
        end
        println(round(value(crop_total_area[year, crop]), digits = 2))
    end
end

println("\nRotation rules at their limit")

if use_return_period
    println("\nReturn period")
    println(rpad("year", 8), rpad("block", 8), "crop")
    for year in years, block in block_names, crop in crop_names
        constraint = return_period_c[year, block, crop]
        if isapprox(value(constraint), normalized_rhs(constraint))
            println(rpad(year, 8), rpad(block, 8), crop)
        end
    end
end

if use_preceding_crop
    println("\nPreceding crop")
    println(rpad("year", 8), "block")
    for year in years, block in block_names
        constraint = preceding_crop_c[year, block]
        if isapprox(value(constraint), normalized_rhs(constraint))
            println(rpad(year, 8), block)
        end
    end
end

if use_legume
    println("\nLegume")
    println("year")
    for year in years
        constraint = legume_c[year]
        if isapprox(value(constraint), normalized_rhs(constraint))
            println(year)
        end
    end
end

original_return_period = normalized_rhs.(return_period_c)
original_preceding_crop = normalized_rhs.(preceding_crop_c)
original_legume = normalized_rhs.(legume_c)

cases = [
    ("all rules", true, true, true),
    ("without return period", false, true, true),
    ("without preceding crop", true, false, true),
    ("without legume", true, true, false),
    ("only return period", true, false, false),
    ("only preceding crop", false, true, false),
    ("only legume", false, false, true),
    ("without any rule", false, false, false),
]

set_silent(model)

case_margins = []
case_years = []
for (label, with_return, with_preceding, with_legume) in cases
    set_normalized_rhs.(return_period_c, with_return ? original_return_period : 1e6)
    set_normalized_rhs.(preceding_crop_c, with_preceding ? original_preceding_crop : 1e6)
    set_normalized_rhs.(legume_c, with_legume ? original_legume : 0)
    optimize!(model)
    push!(case_margins, objective_value(model))
    push!(case_years, [value(profit[year]) for year in years])
end

println("\nCases (EUR)")
println(rpad("case", 26), rpad("year 1", 12), rpad("year 2", 12), rpad("year 3", 12), rpad("discounted", 14), rpad("cost", 12), "cost, %")
for k in eachindex(cases)
    cost = case_margins[end] - case_margins[k]
    print(rpad(cases[k][1], 26))
    for year in years
        print(rpad(round(case_years[k][year], digits = 2), 12))
    end
    print(rpad(round(case_margins[k], digits = 2), 14))
    print(rpad(round(cost, digits = 2), 12))
    println(round(100 * cost / case_margins[end], digits = 2))
end

set_normalized_rhs.(return_period_c, original_return_period)
set_normalized_rhs.(preceding_crop_c, original_preceding_crop)
set_normalized_rhs.(legume_c, original_legume)
optimize!(model)

base_margin = objective_value(model)

tight_constraints = []
for group in (abstraction_permit_c, labour_limit_c, hired_cap_c, contract_max_c, land_c)
    for constraint in group
        if isapprox(value(constraint), normalized_rhs(constraint))
            push!(tight_constraints, constraint)
        end
    end
end

results = []
for constraint in tight_constraints
    rhs = normalized_rhs(constraint)
    extra = 0.1 * rhs
    set_normalized_rhs(constraint, rhs + extra)
    optimize!(model)
    push!(results, (name = name(constraint), extra = extra, gain = objective_value(model) - base_margin))
    set_normalized_rhs(constraint, rhs)
end
optimize!(model)

println("\nInvestments: a constraint at its limit is increased by 10% (EUR, discounted, one year)")
println(rpad("constraint", 36), rpad("extra", 12), rpad("gain", 14), "gain per unit")
for result in sort(results, by = result -> result.gain, rev = true)
    println(rpad(result.name, 36), rpad(round(result.extra, digits = 2), 12), rpad(round(result.gain, digits = 2), 14), round(result.gain / result.extra, digits = 2))
end

# ======= Investments ======= #

autumn = periods[end]

function rent_land(block, extra)
    for year in years
        set_normalized_rhs(land_c[year, block], block_data[block].area_ha + extra)
        set_normalized_rhs(non_productive_minimum_c[year], rules_data.non_productive_share * (farm_area_ha + extra))
        set_normalized_rhs(legume_c[year], rules_data.legume_share * (farm_area_ha + extra))
        for crop in crop_names
            set_normalized_rhs(diversification_c[year, crop], crop_data[crop].max_farm_share * (farm_area_ha + extra))
            set_normalized_rhs(return_period_c[year, block, crop], original_return_period[year, block, crop] + crop_data[crop].return_share * extra)
        end
    end
end

function enlarge_reservoir(extra)
    for year in years, period in periods
        set_upper_bound(level[year, period], water_data.reservoir_capacity_m3 + extra)
    end
end

function add_autumn_hours(extra)
    for year in years
        set_normalized_rhs(labour_limit_c[year, autumn], labour_data[autumn].family_hours + extra)
    end
end

function add_permit(extra)
    for year in years
        set_normalized_rhs(abstraction_permit_c[year], water_data.abstraction_licence_m3 + extra)
    end
end

investments = []
for block in block_names
    push!(investments, ("rent of land on " * string(block) * ", ha", extra -> rent_land(block, extra), 0:1:30))
end
push!(investments, ("reservoir, m3", enlarge_reservoir, 0:250:5000))
push!(investments, ("family hours in autumn, h per year", add_autumn_hours, 0:10:400))
push!(investments, ("abstraction permit, m3 per year", add_permit, 0:250:5000))


first_values = []
for (title, change, amounts) in investments
    gains = []
    for amount in amounts
        change(amount)
        optimize!(model)
        push!(gains, objective_value(model) - base_margin)
    end
    change(0)
    optimize!(model)

    values = diff(gains) ./ diff(amounts) ./ sum(discount_factors)

    println("\n", title)
    println(rpad("from", 10), rpad("up to", 10), "value per unit and year")
    piece_start = 1
    for k in 2:length(values) + 1
        if k > length(values) || !isapprox(values[k], values[piece_start], rtol = 0.02, atol = 1e-3)
            println(rpad(amounts[piece_start], 10), rpad(amounts[k], 10), round(values[piece_start], digits = 3))
            piece_start = k
        end
    end
    push!(first_values, (title = title, value = values[1], total_gain = gains[end]))
end

println("\nValue of the first unit, the highest price worth paying")
println(rpad("investment", 38), "EUR per unit and year")
for item in sort(first_values, by = item -> item.value, rev = true)
    println(rpad(item.title, 38), round(item.value, digits = 3))
end
