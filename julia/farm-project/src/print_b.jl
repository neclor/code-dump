include("b.jl")

println("# ======= B ======= #\n")

# 3.

println("Primal objective: ", objective_value(model))
println("Dual objective:   ", dual_objective_value(model))
println()

println("Prices of all resources:\n")

for b in block_names
    println("land ", b, ": ", -dual(land_c[b]))
end
println()

for c in crop_names
    println("max farm share ", c, ": ", -dual(diversification_c[c]))
end
println()

println("non-productive minimum: ", -dual(non_productive_minimum_c), "\n")

for c in crops_with_contract_min
    println("contract min ", c, ": ", -dual(contract_min_c[c]))
end
println()

for c in crops_with_contract_max
    println("contract max ", c, ": ", -dual(contract_max_c[c]))
end
println()

for c in crops_for_sale
    println("sales limit ", c, ": ", -dual(sales_limit_c[c]))
end
println()

println("abstraction permit: ", -dual(abstraction_permit_c), "\n")

for p in periods
    println("water balance ", p, ": ", -dual(reservoir_balance_c[p]))
end
println()

println("reservoir terminal level: ", -dual(reservoir_terminal_c), "\n")

for p in periods
    println("family hours ", p, ": ", -dual(labour_limit_c[p]))
end
println()

for p in periods
    println("hired cap ", p, ": ", -dual(hired_cap_c[p]))
end
println()

for p in periods
    println("reservoir capacity ", p, ": ", -dual(UpperBoundRef(level[p])))
end
println()

for p in periods
    println("pump capacity ", p, ": ", -dual(UpperBoundRef(pumped[p])))
end
println()

# 4.

set_silent(model)
init_abstraction_licence = water_data.abstraction_licence_m3

prev_profit = nothing
for L in 9000:100:11500
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

