# Stage 2: burner drills on iron, each dropping straight into a stone furnace.
def have(proto):
    return inspect_inventory()[proto]

def ensure(proto, n):
    missing = n - have(proto)
    if missing > 0:
        craft_item(proto, quantity=missing)

# stone for the furnaces
if have(Prototype.Stone) < 20:
    sp = nearest(Resource.Stone)
    move_to(sp)
    harvest_resource(sp, quantity=20)

iron_pos = nearest(Resource.IronOre)
move_to(iron_pos)
ensure(Prototype.BurnerMiningDrill, 3)
ensure(Prototype.StoneFurnace, 3)
print('crafted', inspect_inventory())

drills = []
for i in range(3):
    pos = Position(x=iron_pos.x + 2 * i, y=iron_pos.y)
    try:
        d = place_entity(Prototype.BurnerMiningDrill, direction=Direction.DOWN, position=pos)
        d = insert_item(Prototype.Coal, d, quantity=5)
        f = place_entity(Prototype.StoneFurnace, position=d.drop_position)
        f = insert_item(Prototype.Coal, f, quantity=3)
        drills.append((d, f))
        print('drill', d.position, 'furnace', f.position, d.status)
    except Exception as e:
        print('place failed at', pos, str(e)[:150])
print('inv', inspect_inventory())
