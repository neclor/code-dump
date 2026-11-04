def collect_plates():
    total = 0
    for f in get_entities({Prototype.StoneFurnace}):
        for proto in (Prototype.IronPlate, Prototype.CopperPlate):
            try:
                n = f.furnace_result[proto]
                if n:
                    if f.position.distance(player_location) > 8:
                        move_to(Position(x=f.position.x, y=f.position.y - 2))
                    total += extract_item(proto, f, quantity=n)
            except Exception:
                pass
    return total

print('plates collected', collect_plates(), inspect_inventory())
gather(Resource.Coal, 50)
gather(Resource.Wood, 8)
craft_item(Prototype.WoodenChest, quantity=2)
n_drills = min(2, inv(Prototype.IronPlate) // 9)
print('can build coal drills:', n_drills)
if n_drills:
    craft_item(Prototype.BurnerMiningDrill, quantity=n_drills)
    ref = nearest(Resource.Coal)
    move_to(ref)
    for i in range(n_drills):
        d = place_entity(Prototype.BurnerMiningDrill, direction=Direction.DOWN, position=Position(x=ref.x + 3 * i, y=ref.y))
        d = insert_item(Prototype.Coal, d, quantity=5)
        place_entity(Prototype.WoodenChest, position=d.drop_position)
        print('coal drill', d.position)
print(inspect_inventory())
