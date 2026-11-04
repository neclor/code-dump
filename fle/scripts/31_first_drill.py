def furnaces():
    return get_entities({Prototype.StoneFurnace})

def collect_plates():
    total = 0
    for f in furnaces():
        for proto in (Prototype.IronPlate, Prototype.CopperPlate):
            try:
                n = f.furnace_result[proto]
                if n:
                    if f.position.distance(player_location) > 8:
                        move_to(Position(x=f.position.x + 2, y=f.position.y))
                    total += extract_item(proto, f, quantity=n)
            except Exception:
                pass
    return total

for f in furnaces():
    insert_item(Prototype.IronOre, f, quantity=min(10, inv(Prototype.IronOre)))
sleep(10)
print('plates', collect_plates(), inspect_inventory())
