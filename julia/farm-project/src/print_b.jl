include("a.jl")

println("# ======= B ======= #\n")

# 3.

println("Primal objective: ", objective_value(model))
println("Dual objective:   ", dual_objective_value(model))
println()

println("Prices of all resources:\n")

for b in block_names
    println("land ", b, ": ", shadow_price(land_c[b]))
end
println()

for c in crop_names
    println("max farm share ", c, ": ", shadow_price(diversification_c[c]))
end
println()

println("non-productive minimum: ", shadow_price(non_productive_minimum_c), "\n")

for c in crops_with_contract_min
    println("contract min ", c, ": ", shadow_price(contract_min_c[c]))
end
println()

for c in crops_with_contract_max
    println("contract max ", c, ": ", shadow_price(contract_max_c[c]))
end
println()

for c in crops_for_sale
    println("sales limit ", c, ": ", shadow_price(sales_limit_c[c]))
end
println()

println("abstraction permit: ", shadow_price(abstraction_permit_c), "\n")

for p in periods
    println("water balance ", p, ": ", shadow_price(reservoir_balance_c[p]))
end
println()

println("reservoir terminal level: ", shadow_price(reservoir_terminal_c), "\n")

for p in periods
    println("family hours ", p, ": ", shadow_price(labour_limit_c[p]))
end
println()

for p in periods
    println("hired cap ", p, ": ", shadow_price(hired_cap_c[p]))
end
println()

for p in periods
    println("reservoir capacity ", p, ": ", shadow_price(UpperBoundRef(level[p])))
end
println()

for p in periods
    println("pump capacity ", p, ": ", shadow_price(UpperBoundRef(pumped[p])))
end
println()

# 4.

set_silent(model)
init_abstraction_licence = water_data.abstraction_licence_m3

prev_profit = nothing
for L in 8000:100:11500
    set_normalized_rhs(abstraction_permit_c, L)
    optimize!(model)
    profit_now = objective_value(model)
    if prev_profit !== nothing
        println(L, "  profit = ", round(profit_now, digits = 2), "  slope = ", round((profit_now - prev_profit) / 100, digits = 4))
    end
    global prev_profit = profit_now
end

set_normalized_rhs(abstraction_permit_c, init_abstraction_licence)
optimize!(model)

sensitivity_report = lp_sensitivity_report(model)
println("\nSensitivity report for permit: ", sensitivity_report[abstraction_permit_c])

decrease, increase = sensitivity_report[abstraction_permit_c]
println("L from ", init_abstraction_licence + decrease, " to ", init_abstraction_licence + increase)
println()

L_low = init_abstraction_licence
L_middle = init_abstraction_licence + 250
L_high = init_abstraction_licence + 500

areas = []
for L in (L_low, L_middle, L_high)
    set_normalized_rhs(abstraction_permit_c, L)
    optimize!(model)
    push!(areas, [value(crop_areas[block, crop]) for block in block_names, crop in crop_names])
end

println("Areas that move when L grows (ha per 1000 m3), L from ", L_low, " to ", L_high)
println(rpad("block", 8), rpad("crop", 16), "rate")
for (i, block) in enumerate(block_names), (j, crop) in enumerate(crop_names)
    rate = 1000 * (areas[3][i, j] - areas[1][i, j]) / (L_high - L_low)
    if abs(rate) > 1e-6
        println(rpad(block, 8), rpad(crop, 16), round(rate, digits = 4))
    end
end
println("Check: the middle point is off the straight line by at most ", maximum(abs.(areas[2] .- (areas[1] .+ areas[3]) ./ 2)), " ha")
println()

set_normalized_rhs(abstraction_permit_c, init_abstraction_licence)
optimize!(model)

# 5a.

function solve_from_scratch(method)
    m, _ = copy_model(model)
    set_optimizer(m, Gurobi.Optimizer)
    set_silent(m)
    set_attribute(m, "Presolve", 0)
    set_attribute(m, "Method", method)
    optimize!(m)
    return objective_value(m), MOI.get(m, MOI.SimplexIterations()), solve_time(m)
end

for (name, method) in (("primal simplex", 0), ("dual simplex", 1))
    obj, iterations, t = solve_from_scratch(method)
    println(name, ": objective = ", obj, ", iterations = ", iterations, ", time = ", t, " s")
    println()
end

# 5b.

family_hours_init = labour_data[P4_autumn].family_hours

function reoptimize(method)
    set_attribute(model, "Presolve", 0)
    set_attribute(model, "Method", method)

    set_normalized_rhs(labour_limit_c[P4_autumn], family_hours_init)
    optimize!(model)

    set_normalized_rhs(labour_limit_c[P4_autumn], family_hours_init + 10)
    optimize!(model)

    return (objective_value(model), MOI.get(model, MOI.SimplexIterations()), solve_time(model))
end

for (name, method) in (("primal simplex", 0), ("dual simplex", 1))
    obj, iterations, t = reoptimize(method)
    println("+10 h autumn, ", name, ": objective = ", obj, ", iterations = ", iterations, ", time = ", t, " s")
    println()
end

