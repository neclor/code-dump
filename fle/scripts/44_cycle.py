PACK_CHEST = Position(x=-8.5, y=27.5)
BOILER_CHEST = Position(x=-10.5, y=30.5)

def feed_packs(n):
    if inv(Prototype.IronPlate) < 2 * n + 60 or inv(Prototype.CopperPlate) < n + 20:
        return 0
    craft_item(Prototype.IronGearWheel, quantity=n)
    craft_item(Prototype.AutomationSciencePack, quantity=n)
    go(Position(x=PACK_CHEST.x + 2, y=PACK_CHEST.y + 2))
    insert_item(Prototype.AutomationSciencePack, get_entity(Prototype.IronChest, PACK_CHEST), quantity=n)
    return n

def feed_boiler(reserve=15):
    spare = inv(Prototype.Coal) - reserve
    if spare > 0:
        go(Position(x=BOILER_CHEST.x + 2, y=BOILER_CHEST.y + 2))
        insert_item(Prototype.Coal, get_entity(Prototype.IronChest, BOILER_CHEST), quantity=spare)

def cycle(packs=20):
    k, p = tend()
    feed_boiler()
    n = feed_packs(packs)
    i = inspect_inventory()
    print('coal+%d plates+%d packs+%d | iron %d cu %d coal %d' % (k, p, n, i[Prototype.IronPlate], i[Prototype.CopperPlate], i[Prototype.Coal]))

cycle()
