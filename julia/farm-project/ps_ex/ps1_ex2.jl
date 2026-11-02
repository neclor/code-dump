using JuMP, Gurobi, CSV, DataFrames

# Data
hours = 1:24
units = ["A", "B", "C"]
num_units = length(units)
capacity = [50, 30, 40]  # Capacity for each unit in MW
cost = [20, 25, 18]      # Cost per MW for each unit

# Read demand data from a CSV file
demand_df = CSV.read(joinpath(@__DIR__, "demand_data.csv"), DataFrame)
demand = [demand_df[hour, 1] for hour in hours]  # Demand for each hour

# Create a new JuMP model with Gurobi as the solver
model = Model(Gurobi.Optimizer)

# Variables: Create a matrix of variables where x[i,j] represents unit i's usage at hour j
@variable(model, 0 <= x[1:num_units, hours] <= 1)

# Constraints
for hour in hours
    @constraint(model, sum(capacity[unit_idx] * x[unit_idx, hour] for unit_idx in 1:num_units) >= demand[hour])
end

# Objective: Minimize total operating cost
@objective(model, Min, sum(cost[unit_idx] * x[unit_idx, hour] for unit_idx in 1:num_units, hour in hours))

# Solve the model
optimize!(model)

# Print the results for each unit
if termination_status(model) == MOI.OPTIMAL
    println("Optimal solution found")
    for unit_idx in 1:num_units
        use_percentage = [round(value(x[unit_idx, hour]), digits=3) for hour in 1:24]
        println("Unit ", units[unit_idx], ": ", use_percentage)
    end

else
    println("No optimal solution found")
end
