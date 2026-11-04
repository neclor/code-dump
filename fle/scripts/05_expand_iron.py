def place_drills(resource, count):
    """Place `count` burner drills (each with an output furnace) on free tiles of the nearest patch."""
    ref = nearest(resource)
    patch = get_resource_patch(resource, ref, radius=25)
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
                f = place_entity(Prototype.StoneFurnace, position=d.drop_position)
                insert_item(Prototype.Coal, f, quantity=3)
                placed += 1
            except Exception as e:
                pass
    return placed

n = 4
if inv(Prototype.Stone) < 10 * n:
    gather(Resource.Stone, 10 * n + 10)
craft_item(Prototype.BurnerMiningDrill, quantity=n)
craft_item(Prototype.StoneFurnace, quantity=n)
if inv(Prototype.Coal) < 40:
    gather(Resource.Coal, 40)
print('placed iron drills', place_drills(Resource.IronOre, n))
print(inspect_inventory())
