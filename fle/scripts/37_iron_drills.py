def place_drills(resource, count, chest=False):
    ref = nearest(resource)
    bb = get_resource_patch(resource, ref, radius=25).bounding_box
    placed = 0
    xs = [bb.left_top.x + 2 + 4 * i for i in range(int((bb.right_bottom.x - bb.left_top.x) // 4))]
    ys = [bb.left_top.y + 2 + 4 * j for j in range(int((bb.right_bottom.y - bb.left_top.y) // 4))]
    for yy in ys:
        for xx in xs:
            if placed >= count:
                return placed
            pos = Position(x=xx, y=yy)
            try:
                go(Position(x=xx, y=yy + 3))
                if not can_place_entity(Prototype.BurnerMiningDrill, Direction.DOWN, pos):
                    continue
                d = place_entity(Prototype.BurnerMiningDrill, direction=Direction.DOWN, position=pos)
                d = insert_item(Prototype.Coal, d, quantity=4)
                if chest:
                    place_entity(Prototype.WoodenChest, position=d.drop_position)
                else:
                    f = place_entity(Prototype.StoneFurnace, position=d.drop_position)
                    insert_item(Prototype.Coal, f, quantity=3)
                placed += 1
            except Exception:
                pass
    return placed

n = 5
gather(Resource.Stone, 45)
craft_item(Prototype.BurnerMiningDrill, quantity=n)
craft_item(Prototype.StoneFurnace, quantity=n)
print('placed', place_drills(Resource.IronOre, n))
print(inspect_inventory())

