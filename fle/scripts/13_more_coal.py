n = 8
gather(Resource.Coal, 30)
gather(Resource.Stone, 50)
craft_item(Prototype.BurnerMiningDrill, quantity=n)
craft_item(Prototype.WoodenChest, quantity=n)
print('coal drills', place_coal_drills(n))
# boiler fuel check
b = get_entity(Prototype.Boiler, Position(x=-12.5, y=30.0))
print('boiler fuel', b.fuel)
print(inspect_inventory())
