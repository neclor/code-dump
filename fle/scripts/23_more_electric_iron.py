N = 8
old = get_entities({Prototype.ElectricMiningDrill})
craft_item(Prototype.IronGearWheel, quantity=5 * N) if inv(Prototype.IronGearWheel) < 5 * N else None
craft_item(Prototype.ElectronicCircuit, quantity=3 * N) if inv(Prototype.ElectronicCircuit) < 3 * N else None
craft_item(Prototype.ElectricMiningDrill, quantity=N)
if inv(Prototype.StoneFurnace) < N:
    gather(Resource.Stone, 5 * N + 10)
    craft_item(Prototype.StoneFurnace, quantity=N)

ref = nearest(Resource.IronOre)
bb = get_resource_patch(Resource.IronOre, ref, radius=25).bounding_box
new = []
y = bb.left_top.y + 2
while y < bb.right_bottom.y - 2 and len(new) < N:
    x = bb.left_top.x + 2
    while x < bb.right_bottom.x - 2 and len(new) < N:
        pos = Position(x=x, y=y)
        try:
            move_to(Position(x=x, y=y + 4))
            if can_place_entity(Prototype.ElectricMiningDrill, Direction.DOWN, pos):
                d = place_entity(Prototype.ElectricMiningDrill, direction=Direction.DOWN, position=pos)
                try:
                    f = place_entity(Prototype.StoneFurnace, position=d.drop_position)
                    insert_item(Prototype.Coal, f, quantity=10)
                except Exception:
                    pass
                new.append(d)
        except Exception:
            pass
        x += 4
    y += 6
print('new drills', len(new))
anchor = min(old, key=lambda o: o.position.distance(new[0].position)) if old and new else None
for d in new:
    src = anchor if d is new[0] else min(old + new[:new.index(d)], key=lambda o: o.position.distance(d.position))
    try:
        connect_entities(src, d, Prototype.SmallElectricPole)
    except Exception as e:
        print('link fail', d.position, str(e)[:80])
print([(d.position.x, d.status.name) for d in get_entities({Prototype.ElectricMiningDrill})])
