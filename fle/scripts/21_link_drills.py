craft_item(Prototype.SmallElectricPole, quantity=2 * inv(Prototype.Wood))
print('poles', inv(Prototype.SmallElectricPole))
ds = get_entities({Prototype.ElectricMiningDrill})
ds.sort(key=lambda d: (d.position.y, d.position.x))
print('drills', len(ds), [d.status.name for d in ds])
for a, b in zip(ds, ds[1:]):
    try:
        connect_entities(a, b, Prototype.SmallElectricPole)
    except Exception as e:
        print('link fail', a.position, b.position, str(e)[:100])
for d in get_entities({Prototype.ElectricMiningDrill}):
    print(d.position, d.status.name)
