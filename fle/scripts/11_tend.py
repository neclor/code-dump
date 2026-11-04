RESEARCH_ORDER = [
    Technology.Logistics, Technology.LogisticsSciencePack, Technology.SteelProcessing,
    Technology.Electronics, Technology.FastInserter, Technology.AdvancedMaterialProcessing,
    Technology.ElectricEnergy, Technology.OilProcessing, Technology.Plastics,
    Technology.AdvancedElectronics, Technology.SulfurProcessing,
]

def research_next():
    for t in RESEARCH_ORDER:
        try:
            set_research(t)
            return t
        except Exception:
            continue
    return None

def collect_coal():
    got = 0
    for c in get_entities({Prototype.WoodenChest}):
        n = c.inventory[Prototype.Coal] if c.inventory else 0
        if n:
            move_to(c.position.right().right())
            got += extract_item(Prototype.Coal, c, quantity=n)
    return got

def feed_science(iron=60, copper=40):
    for a in get_entities({Prototype.AssemblingMachine1}):
        r = a.recipe.name if a.recipe else None
        if r == 'iron-gear-wheel' and inv(Prototype.IronPlate) >= iron:
            move_to(a.position.right().right().right())
            insert_item(Prototype.IronPlate, a, quantity=iron)
        elif r == 'automation-science-pack' and inv(Prototype.CopperPlate) >= copper:
            move_to(a.position.right().right().right())
            insert_item(Prototype.CopperPlate, a, quantity=copper)

def tend():
    c = collect_plates()
    k = collect_coal()
    refuel()
    feed_science()
    print('tend: plates', c, 'coal', k, inspect_inventory())

print('research', research_next())
tend()
