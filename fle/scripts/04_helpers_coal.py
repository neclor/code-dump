def inv(p):
    return inspect_inventory()[p]

def collect_plates():
    """Take finished plates out of every furnace."""
    total = 0
    for f in get_entities({Prototype.StoneFurnace}):
        out = f.furnace_result
        for proto in (Prototype.IronPlate, Prototype.CopperPlate, Prototype.StoneBrick):
            n = out[proto] if out else 0
            if n:
                move_to(f.position.right().right())
                total += extract_item(proto, f, quantity=n)
    return total

def refuel(min_coal=3, top_up=5):
    """Top up coal in drills and furnaces that are low."""
    for e in get_entities({Prototype.BurnerMiningDrill, Prototype.StoneFurnace}):
        if inv(Prototype.Coal) < top_up:
            break
        fuel = e.fuel[Prototype.Coal] if e.fuel else 0
        if fuel < min_coal:
            move_to(e.position.right().right())
            insert_item(Prototype.Coal, e, quantity=top_up)

def gather(resource, qty):
    pos = nearest(resource)
    move_to(pos)
    return harvest_resource(pos, quantity=qty)

print('coal', gather(Resource.Coal, 60), inspect_inventory())
