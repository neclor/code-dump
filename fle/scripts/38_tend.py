# needs go/near/collect_plates from 39_sync_location.py (run 39 first, or after this file)
def collect_coal():
    got = 0
    for c in get_entities({Prototype.WoodenChest}):
        try:
            n = c.inventory[Prototype.Coal]
            if n:
                near(c.position, 2)
                got += extract_item(Prototype.Coal, c, quantity=n)
        except Exception:
            pass
    return got

def refuel(min_coal=2, top_up=5):
    for e in get_entities({Prototype.BurnerMiningDrill, Prototype.StoneFurnace}):
        if inv(Prototype.Coal) < top_up:
            break
        try:
            if (e.fuel[Prototype.Coal] if e.fuel else 0) < min_coal:
                near(e.position)
                insert_item(Prototype.Coal, e, quantity=top_up)
        except Exception:
            pass

def tend():
    k = collect_coal()
    p = collect_plates()
    refuel()
    return k, p

print('tend', tend(), inspect_inventory())
