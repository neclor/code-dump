_title(text) = println("\n", "="^78, "\n", text, "\n", "="^78)

# Left aligned text and right aligned rounded number, both padded to `width`.
_text(x, width) = rpad(string(x), width)
_num(x, width; digits = 2) = lpad(abs(x) >= 1e6 ? string(round(Int, x)) : string(round(x; digits = digits) + 0.0), width)

# ======= Gross margin ======= #

_title("Gross margin")
println(_text("Revenue", 24), _num(value(revenue), 14), " EUR")
println(_text("Variable crop cost", 24), _num(-value(crop_cost), 14), " EUR")
println(_text("Pumping cost", 24), _num(-value(pumping_cost), 14), " EUR")
println(_text("Hired labour cost", 24), _num(-value(hire_cost), 14), " EUR")
println(_text("Gross margin", 24), _num(objective_value(model), 14), " EUR")

# ======= Cropping plan ======= #

_title("Cropping plan (ha)")
println(_text("block", 6), join([lpad(crop, 14) for crop in crop_names]), lpad("total", 9))

for block in block_names
    areas = [value(crop_areas[block, crop]) for crop in crop_names]
    println(_text(block, 6), join([_num(area, 14) for area in areas]), _num(sum(areas), 9))
end

farm_areas = [value(crop_total_area[crop]) for crop in crop_names]
println(_text("farm", 6), join([_num(area, 14) for area in farm_areas]), _num(sum(farm_areas), 9))
println(_text("share, %", 6), join([_num(100 * area / farm_area_ha, 14; digits = 1) for area in farm_areas]))

# ======= Production and sales ======= #

_title("Production and sales (tonnes)")
println(_text("crop", 15), lpad("produced", 11), lpad("main price", 12), lpad("spot", 10), lpad("unsold", 10), lpad("revenue EUR", 14))
for crop in crops_for_sale
    market = crop_data[crop].market
    produced = value(crop_production[crop])
    main_sales = value(crop_sales[crop])
    spot_sales = crop in crops_for_spot_sale ? value(crop_spot_sales[crop]) : 0.0
    income = main_sales * market.price_EUR_per_t
    if crop in crops_for_spot_sale
        income += spot_sales * market.spot_price_EUR_per_t
    end
    println(
        _text(crop, 15), _num(produced, 11), _num(main_sales, 12), _num(spot_sales, 10),
        _num(produced - main_sales - spot_sales, 10), _num(income, 14),
    )
end

# ======= Reservoir ======= #

_title("Reservoir (m3)")
println(
    _text("period", 17), lpad("inflow", 9), lpad("pumped", 9), lpad("pump cap", 10),
    lpad("irrigation", 12), lpad("spilled", 9), lpad("level", 9),
)
println(_text("start", 17), " "^(9 + 9 + 10 + 12 + 9), _num(water_data.reservoir_initial_m3, 9; digits = 0))
for period in periods
    println(
        _text(period, 17),
        _num(water_data.periods[period].reservoir_inflow_m3, 9; digits = 0),
        _num(value(pumped[period]), 9; digits = 0),
        _num(water_data.periods[period].pumping_cap_m3, 10; digits = 0),
        _num(value(irrigation[period]), 12; digits = 0),
        _num(value(spilled[period]), 9; digits = 0),
        _num(value(level[period]), 9; digits = 0),
    )
end
println(
    "Pumped in the year: ", round(sum(value(pumped[period]) for period in periods); digits = 0),
    " of ", water_data.abstraction_licence_m3, " m3 (abstraction permit)",
)
println(
    "Reservoir capacity: ", water_data.reservoir_capacity_m3,
    " m3, terminal level at least ", water_data.reservoir_terminal_m3, " m3",
)

# ======= Labour ======= #

_title("Labour (hours)")
println(_text("period", 17), lpad("needed", 9), lpad("family", 9), lpad("hired", 9), lpad("hire cap", 10), lpad("hire EUR", 10))
for period in periods
    println(
        _text(period, 17),
        _num(value(labour_demand[period]), 9; digits = 1),
        _num(labour_data[period].family_hours, 9; digits = 1),
        _num(value(hired[period]), 9; digits = 1),
        _num(labour_data[period].hired_hours_cap, 10; digits = 1),
        _num(value(hired[period]) * labour_data[period].hired_cost_EUR_per_h, 10; digits = 1),
    )
end

# ======= Resources ======= #

# (name, used, limit): the resource is exhausted when used reaches limit.
maximums = Tuple{String, Float64, Float64}[]
# (name, amount, minimum): the requirement is tight when amount is down to the minimum.
minimums = Tuple{String, Float64, Float64}[]

push!(maximums, ("abstraction permit", sum(value(pumped[period]) for period in periods), water_data.abstraction_licence_m3))

for period in periods
    push!(maximums, ("pump capacity $period", value(pumped[period]), water_data.periods[period].pumping_cap_m3))
    push!(maximums, ("reservoir capacity $period", value(level[period]), water_data.reservoir_capacity_m3))
    push!(maximums, ("family hours $period", min(value(labour_demand[period]), labour_data[period].family_hours), labour_data[period].family_hours))
    push!(maximums, ("hired hours $period", value(hired[period]), labour_data[period].hired_hours_cap))
end

for crop in crop_names
    push!(maximums, ("max farm share $crop (ha)", value(crop_total_area[crop]), crop_data[crop].max_farm_share * farm_area_ha))
end

for crop in crops_for_sale
    market = crop_data[crop].market
    market.contract_max_t === nothing || push!(maximums, ("contract max $crop (t)", value(crop_sales[crop]), market.contract_max_t))
    market.contract_min_t === nothing || push!(minimums, ("contract min $crop (t)", value(crop_sales[crop]), market.contract_min_t))
end

push!(minimums, ("reservoir terminal level", value(level[periods[end]]), water_data.reservoir_terminal_m3))
push!(minimums, ("non-productive area (ha)", value(non_productive_area), rules_data.non_productive_share * farm_area_ha))

_is_tight(used, limit) = isapprox(used, limit; atol = 1e-6, rtol = 1e-9)

_title("Resources at the optimum")
println("Land: every block is fully sown (equality constraints).")

println("\nEXHAUSTED (used = limit)")
println("  ", _text("resource", 34), lpad("used", 12), lpad("limit", 12))
for (name, used, limit) in maximums
    _is_tight(used, limit) && println("  ", _text(name, 34), _num(used, 12), _num(limit, 12))
end
for (name, amount, minimum) in minimums
    _is_tight(amount, minimum) && println("  ", _text(name, 34), _num(amount, 12), _num(minimum, 12), "  (minimum)")
end

println("\nNOT EXHAUSTED (slack left)")
println("  ", _text("resource", 34), lpad("used", 12), lpad("limit", 12), lpad("slack", 12))
for (name, used, limit) in maximums
    _is_tight(used, limit) || println("  ", _text(name, 34), _num(used, 12), _num(limit, 12), _num(limit - used, 12))
end
for (name, amount, minimum) in minimums
    _is_tight(amount, minimum) || println("  ", _text(name, 34), _num(amount, 12), _num(minimum, 12), _num(amount - minimum, 12), "  (minimum)")
end
