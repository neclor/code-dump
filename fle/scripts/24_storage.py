STORAGE = []

def stash(proto, keep, chest_cap=3200):
    """Move everything above `keep` of `proto` into iron chests next to the base."""
    extra = inv(proto) - keep
    if extra <= 0:
        return
    move_to(Position(x=12.5, y=30.5))
    for chest in STORAGE:
        if extra <= 0:
            return
        try:
            n = min(extra, 1500)
            insert_item(proto, chest, quantity=n)
            extra -= n
        except Exception:
            continue
    while extra > 0:
        craft_item(Prototype.IronChest, quantity=1)
        c = place_entity(Prototype.IronChest, position=Position(x=10.5 + 2 * len(STORAGE), y=28.5))
        STORAGE.append(c)
        n = min(extra, 3000)
        insert_item(proto, c, quantity=n)
        extra -= n

stash(Prototype.CopperPlate, 400)
print(inspect_inventory())
print('chests', [c.position for c in STORAGE])
