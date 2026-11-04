# Opening: gather coal, stone, iron, build a stone furnace and smelt first plates.
coal_pos = nearest(Resource.Coal)
move_to(coal_pos)
print('coal harvested', harvest_resource(coal_pos, quantity=20))

stone_pos = nearest(Resource.Stone)
move_to(stone_pos)
print('stone harvested', harvest_resource(stone_pos, quantity=10))

iron_pos = nearest(Resource.IronOre)
move_to(iron_pos)
print('iron harvested', harvest_resource(iron_pos, quantity=20))
print('inv', inspect_inventory())

# furnace next to the iron patch
craft_item(Prototype.StoneFurnace, quantity=1)
furnace = place_entity(Prototype.StoneFurnace, position=Position(x=iron_pos.x + 3, y=iron_pos.y))
furnace = insert_item(Prototype.Coal, furnace, quantity=5)
furnace = insert_item(Prototype.IronOre, furnace, quantity=10)
print('furnace', furnace.position, furnace.status)
sleep(15)
print('plates', extract_item(Prototype.IronPlate, furnace, quantity=10))
print('inv', inspect_inventory())
