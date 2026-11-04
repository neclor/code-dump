def feed_green(n=30):
    if inv(Prototype.IronPlate) < 6 * n + 100 or inv(Prototype.CopperPlate) < 2 * n + 50:
        return
    try:
        craft_item(Prototype.IronGearWheel, quantity=3 * n)
        craft_item(Prototype.ElectronicCircuit, quantity=n)
        craft_item(Prototype.Inserter, quantity=n)
        craft_item(Prototype.TransportBelt, quantity=n)
        craft_item(Prototype.LogisticsSciencePack, quantity=n)
    except Exception as e:
        print('green craft failed', str(e)[:100])
        return
    move_to(Position(x=PACK_CHEST.x + 2, y=PACK_CHEST.y + 2))
    chest = get_entity(Prototype.IronChest, PACK_CHEST)
    insert_item(Prototype.LogisticsSciencePack, chest, quantity=inv(Prototype.LogisticsSciencePack))

def tend():
    c = collect_plates()
    k = collect_coal()
    refuel()
    feed_science()
    feed_boiler_chest()
    feed_packs()
    feed_green()
    r = ensure_research()
    print('tend: plates', c, 'coal', k, 'research', r, inspect_inventory())

tend()
