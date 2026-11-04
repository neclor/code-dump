gather(Resource.Coal, 40)
gather(Resource.Stone, 40)
iron = nearest(Resource.IronOre)
move_to(iron)
for e in get_entities({Prototype.BurnerMiningDrill, Prototype.StoneFurnace}):
    try:
        if e.position.distance(player_location) > 8:
            move_to(Position(x=e.position.x + 2, y=e.position.y))
        insert_item(Prototype.Coal, e, quantity=4)
    except Exception:
        pass
print('plates', collect_plates(), inspect_inventory())
