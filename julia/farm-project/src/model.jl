

model = Model(HiGHS.Optimizer) #



# ======= Land ======= #

@variable(model, crop_areas[instances(BlockName), instances(CropName)] >= 0)

# Sum of the areas sown on a block must equal to its area.
for block in instances(BlockName)
    @constraint(model, sum(crop_areas[block, :]) == block_data[block].area_ha)
end

# Culture area <= max_farm_share.
for crop in instances(CropName)
    @constraint(model, sum(crop_areas[:, crop]) <= crop_data[crop].max_farm_share * farm_area_ha)
end

# Minimum share of the farm left non-productive.
@constraint(model, sum(crop_areas[:, cover_fallow]) >= rules_data.non_productive_share * farm_area_ha)

# ======= Water ======= #




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


