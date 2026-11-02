using CSV, DataFrames

_blocks_df = CSV.read("data/blocks.csv", DataFrame)
_calendar_df = CSV.read("data/calendar.csv", DataFrame)
_crops_df = CSV.read("data/crops.csv", DataFrame)
_initial_areas_df = CSV.read("data/initial_areas.csv", DataFrame)
_labour_df = CSV.read("data/labour.csv", DataFrame)
_markets_df = CSV.read("data/markets.csv", DataFrame)
_parameters_df = CSV.read("data/parameters.csv", DataFrame)
_water_df = CSV.read("data/water.csv", DataFrame)
_yields_df = CSV.read("data/yields.csv", DataFrame)

_parameter_data::Dict{String, Float64} = Dict(
    row.parameter => row.value for row in eachrow(_parameters_df)
)

# ======= Enums ======= #

@eval @enum BlockName $(Symbol.(unique(_blocks_df.block))...)
@eval @enum Period $(Symbol.(unique(_labour_df.period))...)
@eval @enum SoilClass $(Symbol.(unique(_yields_df.soil_class))...)
@eval @enum CropName $(Symbol.(unique(_crops_df.crop))...)
@eval @enum CropGroup $(Symbol.(unique(_crops_df.group))...)

block_names = instances(BlockName)
crop_names = instances(CropName)
periods = instances(Period)
soil_classes = instances(SoilClass)

function parse_enum(E::Type{<:Enum}, str::AbstractString)
    for enum_value in instances(E)
        string(enum_value) == str && return enum_value
    end
    error("unknown $(nameof(E)): \"$str\"")
end

# ======= Blocks ======= #

struct BlockData
    area_ha::Float64
    soil_class::SoilClass

    initial_areas::Dict{CropName, Float64}
end

_zero_initial_area() = Dict{CropName, Float64}(crop => 0.0 for crop in instances(CropName))

_initial_area_data::Dict{BlockName, Dict{CropName, Float64}} = Dict(
    parse_enum(BlockName, key.block) => merge(
        _zero_initial_area(),
        Dict(parse_enum(CropName, row.crop) => row.area_ha for row in eachrow(group))
    ) for (key, group) in pairs(groupby(_initial_areas_df, :block))
)

block_data::Dict{BlockName, BlockData} = Dict(
    parse_enum(BlockName, row.block) => BlockData(
        row.area_ha,
        parse_enum(SoilClass, row.soil_class),
        get(_initial_area_data, parse_enum(BlockName, row.block), _zero_initial_area())
    ) for row in eachrow(_blocks_df)
)

for (name, b) in block_data
    total = sum(values(b.initial_areas); init = 0.0)
    if !isapprox(total, b.area_ha)
        error("block $name: initial areas sum to $total ha, block area is $(b.area_ha) ha")
    end
end

farm_area_ha::Float64 = sum(b.area_ha for b in values(block_data))

# ======= Crops ======= #

struct CalendarData
    irrigation_m3_per_ha::Float64
    labour_h_per_ha::Float64
end

struct MarketData
    price_EUR_per_t::Float64
    contract_min_t::Union{Nothing, Float64}
    contract_max_t::Union{Nothing, Float64}
    spot_price_EUR_per_t::Union{Nothing, Float64}
end

struct CropData
    group::CropGroup
    variable_cost_EUR_per_ha::Float64
    max_farm_share::Float64
    return_share::Float64
    is_legume::Bool
    is_non_productive::Bool

    yields::Dict{SoilClass, Float64}
    calendar::Dict{Period, CalendarData}
    market::Union{Nothing, MarketData}
end

_yield_data::Dict{CropName, Dict{SoilClass, Float64}} = Dict(
    parse_enum(CropName, key.crop) => Dict(
        parse_enum(SoilClass, row.soil_class) => row.yield_t_per_ha
        for row in eachrow(group)
    ) for (key, group) in pairs(groupby(_yields_df, :crop))
)

_calendar_data::Dict{CropName, Dict{Period, CalendarData}} = Dict(
    parse_enum(CropName, key.crop) => Dict(
        parse_enum(Period, row.period) => CalendarData(row.irrigation_m3_per_ha, row.labour_h_per_ha)
        for row in eachrow(group)
    ) for (key, group) in pairs(groupby(_calendar_df, :crop))
)

_market_data::Dict{CropName, MarketData} = Dict(
    parse_enum(CropName, row.crop) => MarketData(
        row.price_EUR_per_t,
        coalesce(row.contract_min_t, nothing),
        coalesce(row.contract_max_t, nothing),
        coalesce(row.spot_price_EUR_per_t, nothing)
    ) for row in eachrow(_markets_df)
)

crop_data::Dict{CropName, CropData} = Dict(
    parse_enum(CropName, row.crop) => CropData(
        parse_enum(CropGroup, row.group),
        row.variable_cost_EUR_per_ha,
        row.max_farm_share,
        row.return_share,
        row.is_legume == 1,
        row.is_non_productive == 1,

        _yield_data[parse_enum(CropName, row.crop)],
        _calendar_data[parse_enum(CropName, row.crop)],
        get(_market_data, parse_enum(CropName, row.crop), nothing)
    ) for row in eachrow(_crops_df)
)

# ======= Water ======= #

struct WaterPeriodData
    reservoir_inflow_m3::Float64
    pumping_cap_m3::Float64
end

struct WaterData
    reservoir_capacity_m3::Float64
    reservoir_initial_m3::Float64
    reservoir_terminal_m3::Float64
    abstraction_licence_m3::Float64
    pumping_cost_EUR_per_m3::Float64

    periods::Dict{Period, WaterPeriodData}
end

water_data::WaterData = WaterData(
    _parameter_data["reservoir_capacity"],
    _parameter_data["reservoir_initial"],
    _parameter_data["reservoir_terminal"],
    _parameter_data["abstraction_licence"],
    _parameter_data["pumping_cost"],
    Dict(
        parse_enum(Period, row.period) => WaterPeriodData(row.reservoir_inflow_m3, row.pumping_cap_m3)
        for row in eachrow(_water_df)
    )
)

# ======= Labour ======= #

struct LabourData
    family_hours::Float64
    hired_hours_cap::Float64
    hired_cost_EUR_per_h::Float64
end

labour_data::Dict{Period, LabourData} = Dict(
    parse_enum(Period, row.period) => LabourData(row.family_hours, row.hired_hours_cap, row.hired_cost_EUR_per_h)
    for row in eachrow(_labour_df)
)

# ======= Rules ======= #

struct RulesData
    non_productive_share::Float64

    legume_share::Float64
    horizon_years::Int
    discount_rate::Float64
end

rules_data::RulesData = RulesData(
    _parameter_data["non_productive_share"],

    _parameter_data["legume_share"],
    Int(_parameter_data["horizon"]),
    _parameter_data["discount_rate"]
)
