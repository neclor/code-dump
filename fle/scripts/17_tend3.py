PACK_CHEST = Position(x=-6.5, y=27.5)

def feed_packs(n=40):
    """Hand-craft red science from stock plates and drop it into the lab chest."""
    if inv(Prototype.IronPlate) < 2 * n + 100 or inv(Prototype.CopperPlate) < n + 50:
        return
    craft_item(Prototype.IronGearWheel, quantity=n)
    craft_item(Prototype.AutomationSciencePack, quantity=n)
    move_to(Position(x=PACK_CHEST.x + 2, y=PACK_CHEST.y + 2))
    chest = get_entity(Prototype.IronChest, PACK_CHEST)
    insert_item(Prototype.AutomationSciencePack, chest, quantity=inv(Prototype.AutomationSciencePack))

def tend():
    c = collect_plates()
    k = collect_coal()
    refuel()
    feed_science()
    feed_boiler_chest()
    feed_packs()
    r = ensure_research()
    print('tend: plates', c, 'coal', k, 'research', r, inspect_inventory())

tend()
