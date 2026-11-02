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
