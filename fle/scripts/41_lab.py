engine = get_entity(Prototype.SteamEngine, Position(x=-13.5, y=24.5))
lab = None
for dx, dy in ((6, 0), (6, -6), (-6, 0), (0, -9)):
    try:
        spot = Position(x=engine.position.x + dx, y=engine.position.y + dy)
        move_to(Position(x=spot.x, y=spot.y + 3))
        if can_place_entity(Prototype.Lab, Direction.UP, spot):
            lab = place_entity(Prototype.Lab, position=spot)
            break
    except Exception as e:
        print('spot fail', dx, dy, str(e)[:70])
print('lab', lab.position if lab else None)
connect_entities(engine, lab, Prototype.SmallElectricPole)
lab = insert_item(Prototype.AutomationSciencePack, lab, quantity=10)
print('lab', lab.position, lab.status)
LAB_POS = lab.position
print(inspect_inventory())
