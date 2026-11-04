# get_entity/get_entities fail for labs holding science packs (Lua array vs dict in the
# serializer) and place_entity(Lab) raises after placing, so: ignore those errors, and
# share packs between labs with inserters instead of inserting by hand.
def safe_place(proto, pos, direction=Direction.UP):
    try:
        return place_entity(proto, direction=direction, position=pos)
    except Exception as e:
        print('placed with error', proto.value[0], pos, str(e)[:40])

craft_item(Prototype.Inserter, quantity=6)
craft_item(Prototype.WoodenChest, quantity=1) if inv(Prototype.WoodenChest) < 1 and inv(Prototype.Wood) >= 2 else None
craft_item(Prototype.IronChest, quantity=1)
move_to(Position(x=0.5, y=28.5))

safe_place(Prototype.SmallElectricPole, Position(x=-0.5, y=26.5))
safe_place(Prototype.SmallElectricPole, Position(x=3.5, y=26.5))
safe_place(Prototype.Lab, Position(x=1.5, y=24.5))
safe_place(Prototype.Lab, Position(x=5.5, y=24.5))
for gx in (-4.5, -0.5, 3.5):
    safe_place(Prototype.Inserter, Position(x=gx, y=24.5), Direction.RIGHT)

# hand-fed pack chest below the first lab
safe_place(Prototype.SmallElectricPole, Position(x=-8.5, y=27.5))
safe_place(Prototype.Inserter, Position(x=-6.5, y=26.5), Direction.UP)
packs_chest = place_entity(Prototype.IronChest, position=Position(x=-6.5, y=27.5))
packs_chest = insert_item(Prototype.AutomationSciencePack, packs_chest, quantity=inv(Prototype.AutomationSciencePack))
print('packs chest', packs_chest.position, packs_chest.inventory)
print(inspect_inventory())
