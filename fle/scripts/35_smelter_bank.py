BANK = []

def build_bank(n):
    """Stone furnaces in a row next to the iron patch, used for hand-fed smelting."""
    iron = nearest(Resource.IronOre)
    move_to(Position(x=iron.x + 6, y=iron.y + 4))
    craft_item(Prototype.StoneFurnace, quantity=n)
    for i in range(n):
        f = place_entity(Prototype.StoneFurnace, position=Position(x=iron.x + 6 + 2 * i, y=iron.y + 6))
        BANK.append(f.position)

def feed_bank(ore_each, coal_each=3):
    for pos in BANK:
        f = get_entity(Prototype.StoneFurnace, pos)
        if f.position.distance(player_location) > 8:
            move_to(Position(x=f.position.x, y=f.position.y - 2))
        insert_item(Prototype.Coal, f, quantity=coal_each)
        insert_item(Prototype.IronOre, f, quantity=min(ore_each, inv(Prototype.IronOre)))

gather(Resource.Stone, 40)
build_bank(5)
iron = nearest(Resource.IronOre)
move_to(iron)
harvest_resource(iron, quantity=60)
feed_bank(12)
print(inspect_inventory())
