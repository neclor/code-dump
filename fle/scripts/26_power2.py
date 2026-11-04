craft_item(Prototype.OffshorePump, quantity=1)
craft_item(Prototype.Boiler, quantity=1)
craft_item(Prototype.SteamEngine, quantity=2)
craft_item(Prototype.Pipe, quantity=20)
craft_item(Prototype.Inserter, quantity=1)
craft_item(Prototype.IronChest, quantity=1)

X = -22.5
move_to(Position(x=X, y=32.5))
pump = place_entity(Prototype.OffshorePump, position=Position(x=X, y=nearest(Resource.Water).y))
print('pump', pump.position, pump.direction)
boiler = place_entity_next_to(Prototype.Boiler, reference_position=pump.position, direction=Direction.UP, spacing=3)
e1 = place_entity_next_to(Prototype.SteamEngine, reference_position=boiler.position, direction=Direction.UP, spacing=2)
e2 = place_entity_next_to(Prototype.SteamEngine, reference_position=e1.position, direction=Direction.UP, spacing=0)
print('boiler', boiler.position, 'engines', e1.position, e2.position)
print(connect_entities(pump, boiler, Prototype.Pipe))
print(connect_entities(boiler, e1, Prototype.Pipe))
boiler = insert_item(Prototype.Coal, boiler, quantity=30)
# automatic coal feed: chest + inserter on the right side of the boiler
ins = place_entity(Prototype.Inserter, direction=Direction.LEFT, position=Position(x=boiler.position.x + 2, y=boiler.position.y + 0.5))
chest = place_entity(Prototype.IronChest, position=ins.pickup_position)
insert_item(Prototype.Coal, chest, quantity=600)
# join to the main grid and power the inserter
print(connect_entities(Position(x=-9.5, y=27.5), e2, Prototype.SmallElectricPole))
print(connect_entities(e2, ins, Prototype.SmallElectricPole))
sleep(5)
print([(e.position.x, e.position.y, e.status.name) for e in get_entities({Prototype.SteamEngine, Prototype.Boiler})])
BOILER2_CHEST = chest.position
print('boiler2 chest', BOILER2_CHEST)
