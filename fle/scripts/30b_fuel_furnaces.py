gather(Resource.Coal, 25)
iron = nearest(Resource.IronOre)
move_to(iron)
for f in get_entities({Prototype.StoneFurnace}):
    try:
        if f.position.distance(player_location) > 8:
            move_to(Position(x=f.position.x + 2, y=f.position.y))
        insert_item(Prototype.Coal, f, quantity=4)
    except Exception as e:
        print('fuel fail', f.position, str(e)[:60])
harvest_resource(iron, quantity=20)
print('plates', collect_plates(), inspect_inventory())
