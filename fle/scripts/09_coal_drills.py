def place_coal_drills(count):
    ref = nearest(Resource.Coal)
    patch = get_resource_patch(Resource.Coal, ref, radius=25)
    bb = patch.bounding_box
    placed = 0
    xs = [bb.left_top.x + 2 + 4 * i for i in range(int((bb.right_bottom.x - bb.left_top.x) // 4))]
    ys = [bb.left_top.y + 2 + 4 * j for j in range(int((bb.right_bottom.y - bb.left_top.y) // 4))]
    for yy in ys:
        for xx in xs:
            if placed >= count:
                return placed
            pos = Position(x=xx, y=yy)
            try:
                move_to(Position(x=xx, y=yy + 3))
                if not can_place_entity(Prototype.BurnerMiningDrill, Direction.DOWN, pos):
                    continue
                d = place_entity(Prototype.BurnerMiningDrill, direction=Direction.DOWN, position=pos)
                d = insert_item(Prototype.Coal, d, quantity=5)
                place_entity(Prototype.WoodenChest, position=d.drop_position)
                placed += 1
            except Exception:
                pass
    return placed

gather(Resource.Coal, 60)
if inv(Prototype.Stone) < 20:
    gather(Resource.Stone, 25)
n = 4
craft_item(Prototype.BurnerMiningDrill, quantity=n)
craft_item(Prototype.WoodenChest, quantity=n)
print('coal drills', place_coal_drills(n))
print(inspect_inventory())
