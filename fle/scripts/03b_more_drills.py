# Place the remaining burner drills on free ore tiles inside the iron patch.
iron_pos = nearest(Resource.IronOre)
patch = get_resource_patch(Resource.IronOre, iron_pos, radius=20)
bb = patch.bounding_box
print('patch', patch.size, bb.left_top, bb.right_bottom)

placed = 0
y = bb.left_top.y + 2
while y < bb.right_bottom.y - 2 and have_left(Prototype.BurnerMiningDrill) if False else True:
    break

def drills_left():
    return inspect_inventory()[Prototype.BurnerMiningDrill]

x_range = [bb.left_top.x + 2 + 4 * i for i in range(int((bb.right_bottom.x - bb.left_top.x) // 4))]
y_range = [bb.left_top.y + 2 + 4 * j for j in range(int((bb.right_bottom.y - bb.left_top.y) // 4))]
for yy in y_range:
    for xx in x_range:
        if drills_left() == 0:
            break
        pos = Position(x=xx, y=yy)
        try:
            move_to(Position(x=xx, y=yy + 3))
            if not can_place_entity(Prototype.BurnerMiningDrill, Direction.DOWN, pos):
                continue
            d = place_entity(Prototype.BurnerMiningDrill, direction=Direction.DOWN, position=pos)
            d = insert_item(Prototype.Coal, d, quantity=5)
            f = place_entity(Prototype.StoneFurnace, position=d.drop_position)
            f = insert_item(Prototype.Coal, f, quantity=3)
            placed += 1
            print('drill', d.position, 'furnace', f.position, d.status)
        except Exception as e:
            print('skip', pos, str(e)[:90])
print('placed', placed, inspect_inventory())
