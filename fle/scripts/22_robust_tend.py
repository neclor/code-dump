def collect_plates():
    total = 0
    for f in get_entities({Prototype.StoneFurnace}):
        try:
            out = f.furnace_result
            for proto in (Prototype.IronPlate, Prototype.CopperPlate, Prototype.StoneBrick):
                n = out[proto] if out else 0
                if n:
                    move_to(f.position.right().right())
                    total += extract_item(proto, f, quantity=n)
        except Exception as e:
            pass
    return total

def collect_coal():
    got = 0
    for c in get_entities({Prototype.WoodenChest}):
        try:
            n = c.inventory[Prototype.Coal] if c.inventory else 0
            if n and c.position != BOILER_CHEST:
                move_to(c.position.right().right())
                got += extract_item(Prototype.Coal, c, quantity=n)
        except Exception:
            pass
    return got

def refuel(min_coal=3, top_up=10):
    for e in get_entities({Prototype.BurnerMiningDrill, Prototype.StoneFurnace}):
        if inv(Prototype.Coal) < top_up:
            break
        try:
            fuel = e.fuel[Prototype.Coal] if e.fuel else 0
            if fuel < min_coal:
                move_to(e.position.right().right())
                insert_item(Prototype.Coal, e, quantity=top_up)
        except Exception:
            pass

print('collected', collect_plates(), collect_coal())
refuel()
print(inspect_inventory())
print('engines', [(e.position.y, e.status.name) for e in get_entities({Prototype.SteamEngine})])
print('boiler', [b.status.name for b in get_entities({Prototype.Boiler})])
