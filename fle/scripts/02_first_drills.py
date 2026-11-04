# Stage 1: materials -> plates -> 2 burner drills on iron, each dropping straight into a stone furnace.
def gather(resource, qty):
    pos = nearest(resource)
    move_to(pos)
    got = harvest_resource(pos, quantity=qty)
    return pos, got

coal_pos, _ = gather(Resource.Coal, 40)
stone_pos, _ = gather(Resource.Stone, 30)
iron_pos, _ = gather(Resource.IronOre, 40)
print('inv after gather', inspect_inventory())

# smelting stack: 2 furnaces next to the iron patch
craft_item(Prototype.StoneFurnace, quantity=2)
smelters = []
for i in range(2):
    f = place_entity(Prototype.StoneFurnace, position=Position(x=iron_pos.x + 4 + 3 * i, y=iron_pos.y + 4))
    f = insert_item(Prototype.Coal, f, quantity=5)
    f = insert_item(Prototype.IronOre, f, quantity=18)
    smelters.append(f)
print('smelting at', [f.position for f in smelters])
sleep(15)
sleep(15)
got = 0
for f in smelters:
    try:
        got += extract_item(Prototype.IronPlate, f, quantity=20)
    except Exception as e:
        print('extract issue', str(e)[:80])
print('plates', got, inspect_inventory())
