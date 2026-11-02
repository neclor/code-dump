include("a.jl")

println("# ======= A ======= #\n")

println("Gross margin")
println("Revenue: ", round(value(revenue), digits = 2))
println("Crop cost: ", round(value(crop_cost), digits = 2))
println("Pumping cost: ", round(value(pumping_cost), digits = 2))
println("Hire cost: ", round(value(hire_cost), digits = 2))
println("Gross margin: ", round(objective_value(model), digits = 2))

println("\nCropping plan (ha)")
print(rpad("crop", 16))
for block in block_names
    print(rpad(block, 10))
end
println("farm")
for crop in crop_names
    print(rpad(crop, 16))
    for block in block_names
        print(rpad(round(value(crop_areas[block, crop]), digits = 2), 10))
    end
    println(round(value(crop_total_area[crop]), digits = 2))
end

println("\nProduction and sales (t)")
println(rpad("crop", 16), rpad("produced", 12), rpad("sold", 12), "sold on spot")
for crop in crops_for_sale
    print(rpad(crop, 16))
    print(rpad(round(value(crop_production[crop]), digits = 2), 12))
    print(rpad(round(value(crop_sales[crop]), digits = 2), 12))
    println(crop in crops_for_spot_sale ? round(value(crop_spot_sales[crop]), digits = 2) : "-")
end

println("\nReservoir (m3), start level ", water_data.reservoir_initial_m3)
println(rpad("period", 18), rpad("inflow", 10), rpad("pumped", 10), rpad("irrigation", 12), rpad("spilled", 10), "level at the end")
for period in periods
    print(rpad(period, 18))
    print(rpad(water_data.periods[period].reservoir_inflow_m3, 10))
    print(rpad(round(value(pumped[period]), digits = 2), 10))
    print(rpad(round(value(irrigation[period]), digits = 2), 12))
    print(rpad(round(value(spilled[period]), digits = 2), 10))
    println(round(value(level[period]), digits = 2))
end

println("\nLabour (hours)")
println(rpad("period", 18), rpad("needed", 10), rpad("family", 10), "hired")
for period in periods
    print(rpad(period, 18))
    print(rpad(round(value(labour_demand[period]), digits = 2), 10))
    print(rpad(labour_data[period].family_hours, 10))
    println(round(value(hired[period]), digits = 2))
end

limits = []
for constraint in all_constraints(model; include_variable_in_set_constraints = false)
    if !(constraint_object(constraint).set isa MOI.EqualTo)
        push!(limits, (name(constraint), abs(value(constraint) - normalized_rhs(constraint))))
    end
end
for period in periods
    push!(limits, ("pump_capacity[$period]", water_data.periods[period].pumping_cap_m3 - value(pumped[period])))
    push!(limits, ("reservoir_full[$period]", water_data.reservoir_capacity_m3 - value(level[period])))
    push!(limits, ("reservoir_empty[$period]", value(level[period])))
end

println("\nExhausted resources (slack 0)")
for (label, slack) in limits
    if round(slack, digits = 2) == 0
        println(label)
    end
end

println("\nNot exhausted (slack left)")
for (label, slack) in limits
    if round(slack, digits = 2) != 0
        println(rpad(label, 36), round(slack, digits = 2))
    end
end
