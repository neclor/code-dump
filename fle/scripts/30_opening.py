# Real-conditions opening: every step costs game time, so overlap smelting with mining.
def inv(p):
    return inspect_inventory()[p]

def gather(resource, qty):
    pos = nearest(resource)
    move_to(pos)
    return harvest_resource(pos, quantity=qty)

gather(Resource.Stone, 25)            # 5 furnaces worth
iron = nearest(Resource.IronOre)
move_to(iron)
harvest_resource(iron, quantity=15)
craft_item(Prototype.StoneFurnace, quantity=2)
f1 = place_entity(Prototype.StoneFurnace, position=Position(x=iron.x + 2, y=iron.y))
f2 = place_entity(Prototype.StoneFurnace, position=Position(x=iron.x + 2, y=iron.y + 2))
for f in (f1, f2):
    insert_item(Prototype.Coal, f, quantity=3)
insert_item(Prototype.IronOre, f1, quantity=8)
insert_item(Prototype.IronOre, f2, quantity=7)
# keep mining while they smelt
harvest_resource(iron, quantity=20)
print(inspect_inventory())
