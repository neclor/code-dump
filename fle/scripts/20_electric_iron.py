N = 8
craft_item(Prototype.IronGearWheel, quantity=5 * N)
craft_item(Prototype.ElectronicCircuit, quantity=3 * N)
craft_item(Prototype.ElectricMiningDrill, quantity=N)
craft_item(Prototype.StoneFurnace, quantity=N) if inv(Prototype.Stone) >= 5 * N else gather(Resource.Stone, 5 * N + 10)
craft_item(Prototype.StoneFurnace, quantity=N) if inv(Prototype.StoneFurnace) < N else None
print(inspect_inventory())

ref = nearest(Resource.IronOre)
patch = get_resource_patch(Resource.IronOre, ref, radius=25)
bb = patch.bounding_box
drills = []
y = bb.left_top.y + 2
while y < bb.right_bottom.y - 2 and len(drills) < N:
    x = bb.left_top.x + 2
    while x < bb.right_bottom.x - 2 and len(drills) < N:
        pos = Position(x=x, y=y)
        try:
            move_to(Position(x=x, y=y + 4))
            if can_place_entity(Prototype.ElectricMiningDrill, Direction.DOWN, pos):
                d = place_entity(Prototype.ElectricMiningDrill, direction=Direction.DOWN, position=pos)
                try:
                    f = place_entity(Prototype.StoneFurnace, position=d.drop_position)
                    insert_item(Prototype.Coal, f, quantity=10)
                except Exception as fe:
                    print('furnace fail', str(fe)[:60])
                drills.append(d)
        except Exception as e:
            pass
        x += 4
    y += 6
print('electric drills', [d.position for d in drills])

# power line from the plant to the first drill, then daisy-chain
e1 = get_entity(Prototype.SteamEngine, Position(x=-12.5, y=19.5))
grp = connect_entities(e1, drills[0], Prototype.SmallElectricPole)
for a, b in zip(drills, drills[1:]):
    try:
        connect_entities(a, b, Prototype.SmallElectricPole)
    except Exception as e:
        print('link fail', a.position, b.position, str(e)[:80])
print(inspect_inventory())
