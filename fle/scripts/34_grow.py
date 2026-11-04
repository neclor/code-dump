iron = nearest(Resource.IronOre)
for f in get_entities({Prototype.StoneFurnace}):
    try:
        if f.position.distance(player_location) > 8:
            move_to(Position(x=f.position.x + 2, y=f.position.y))
        insert_item(Prototype.IronOre, f, quantity=min(10, inv(Prototype.IronOre)))
    except Exception:
        pass
harvest_resource(iron, quantity=25)   # mining takes game time while the furnaces work
print('plates', collect_plates(), inspect_inventory())
