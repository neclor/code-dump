n = 8
gather(Resource.Stone, 90)
craft_item(Prototype.BurnerMiningDrill, quantity=n)
craft_item(Prototype.StoneFurnace, quantity=n)
print('iron drills', place_drills(Resource.IronOre, n))
refuel()
print(inspect_inventory())
