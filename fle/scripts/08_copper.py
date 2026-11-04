print('collected', collect_plates())
if inv(Prototype.Coal) < 40:
    gather(Resource.Coal, 60)
if inv(Prototype.Stone) < 50:
    gather(Resource.Stone, 50)
n = 4
craft_item(Prototype.BurnerMiningDrill, quantity=n)
craft_item(Prototype.StoneFurnace, quantity=n)
print('placed copper drills', place_drills(Resource.CopperOre, n))
refuel()
print(inspect_inventory())
