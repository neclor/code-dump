gather(Resource.Wood, 40)
craft_item(Prototype.IronGearWheel, quantity=40)
craft_item(Prototype.SteamEngine, quantity=1)
craft_item(Prototype.SmallElectricPole, quantity=30)
craft_item(Prototype.Pipe, quantity=4)

move_to(Position(x=-9.5, y=20.5))
e1 = get_entity(Prototype.SteamEngine, Position(x=-12.5, y=24.5))
try:
    e2 = place_entity_next_to(Prototype.SteamEngine, reference_position=e1.position, direction=Direction.UP, spacing=0)
    print('engine2', e2.position, e2.status)
except Exception as e:
    print('engine2 err', str(e)[:200])
print(inspect_inventory())
