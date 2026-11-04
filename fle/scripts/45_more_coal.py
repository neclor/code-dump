SERVICE_CHESTS = [(-8.5, 27.5), (-10.5, 30.5)]

def collect_coal():
    got = 0
    for c in get_entities({Prototype.WoodenChest, Prototype.IronChest}):
        if (c.position.x, c.position.y) in SERVICE_CHESTS:
            continue
        try:
            n = c.inventory[Prototype.Coal]
            if n:
                near(c.position, 2)
                got += extract_item(Prototype.Coal, c, quantity=n)
        except Exception:
            pass
    return got

def place_coal_drills(count):
    ref = nearest(Resource.Coal)
    bb = get_resource_patch(Resource.Coal, ref, radius=25).bounding_box
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
                place_entity(Prototype.IronChest, position=d.drop_position)
                placed += 1
            except Exception:
                pass
    return placed

n = 4
gather(Resource.Stone, 25)
craft_item(Prototype.BurnerMiningDrill, quantity=n)
craft_item(Prototype.IronChest, quantity=n)
gather(Resource.Coal, 20)
print('coal drills', place_coal_drills(n))
print(inspect_inventory())
