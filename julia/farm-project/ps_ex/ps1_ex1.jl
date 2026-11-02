# import Pkg
# Pkg.add("JuMP")
# Pkg.add("Gurobi")

using JuMP, HiGHS

# Create a new JuMP model with HiGHS as the solver
model = Model(HiGHS.Optimizer)

# variables
@variable(model, production_butter >= 0)
@variable(model, production_ice_cream >= 0)

# constraints
@constraint(model, 7*production_butter + 3*production_ice_cream <= 80)
@constraint(model, production_ice_cream <= 20)
@constraint(model, (1/15)*production_ice_cream + production_butter <= 6)

# objective
@objective(model, Max, 2*production_ice_cream + 7*production_butter)

optimize!(model)


# Print the results for each unit
if termination_status(model) == MOI.OPTIMAL
    println("Optimal solution found")    
    println("Production of butter: ", value(production_butter))
    println("Production of ice cream: ", value(production_ice_cream))
else
    println("No optimal solution found")
end

