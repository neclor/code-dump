lab = get_entity(Prototype.Lab, Position(x=-6.5, y=24.5))
craft_item(Prototype.AssemblingMachine1, quantity=1)
craft_item(Prototype.Inserter, quantity=3)
craft_item(Prototype.SmallElectricPole, quantity=4)
move_to(Position(x=lab.position.x + 5, y=lab.position.y - 4))

sci = place_entity_next_to(Prototype.AssemblingMachine1, reference_position=lab.position, direction=Direction.UP, spacing=1)
sci = set_entity_recipe(sci, Prototype.AutomationSciencePack)
print('science asm', sci.position)
gear = place_entity_next_to(Prototype.AssemblingMachine1, reference_position=sci.position, direction=Direction.UP, spacing=1)
gear = set_entity_recipe(gear, Prototype.IronGearWheel)
print('gear asm', gear.position)

# inserter lab<-science, science<-gear (gap rows between the machines)
ins1 = place_entity(Prototype.Inserter, direction=Direction.DOWN, position=Position(x=lab.position.x, y=(lab.position.y + sci.position.y) / 2))
ins2 = place_entity(Prototype.Inserter, direction=Direction.DOWN, position=Position(x=lab.position.x, y=(sci.position.y + gear.position.y) / 2))
print('ins1', ins1.position, ins1.pickup_position, ins1.drop_position)
print('ins2', ins2.position, ins2.pickup_position, ins2.drop_position)

# power: poles next to the machines, linked to the existing network
for e in (sci, gear):
    print(connect_entities(lab, e, Prototype.SmallElectricPole))

# feed by hand: iron into the gear asm, copper into the science asm
gear = insert_item(Prototype.IronPlate, gear, quantity=60)
sci = insert_item(Prototype.CopperPlate, sci, quantity=40)
sleep(10)
for e, p in ((gear, Prototype.AssemblingMachine1), (sci, Prototype.AssemblingMachine1)):
    print(get_entity(p, e.position).status)
print('lab', get_entity(Prototype.Lab, lab.position).status)
