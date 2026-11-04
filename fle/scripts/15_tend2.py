BOILER_CHEST = Position(x=-9.5, y=30.5)

def feed_boiler_chest(reserve=30):
    spare = inv(Prototype.Coal) - reserve
    if spare > 0:
        move_to(Position(x=BOILER_CHEST.x + 2, y=BOILER_CHEST.y + 2))
        chest = get_entity(Prototype.WoodenChest, BOILER_CHEST)
        insert_item(Prototype.Coal, chest, quantity=spare)

def ensure_research():
    try:
        get_research_progress()
    except Exception:
        return research_next()

def tend():
    c = collect_plates()
    k = collect_coal()
    refuel()
    feed_science()
    feed_boiler_chest()
    r = ensure_research()
    print('tend: plates', c, 'coal', k, 'research', r, inspect_inventory())

tend()
