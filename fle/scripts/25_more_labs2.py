collect_plates()
n = 8
craft_item(Prototype.ElectronicCircuit, quantity=max(0, 10 * n - inv(Prototype.ElectronicCircuit)))
craft_item(Prototype.TransportBelt, quantity=max(0, 4 * n - inv(Prototype.TransportBelt)))
craft_item(Prototype.Lab, quantity=n)
craft_item(Prototype.Inserter, quantity=max(0, n - inv(Prototype.Inserter)))
craft_item(Prototype.SmallElectricPole, quantity=max(0, 2 * n - inv(Prototype.SmallElectricPole)))
move_to(Position(x=8.5, y=28.5))
for i in range(n):
    x = 9.5 + 4 * i
    move_to(Position(x=x, y=28.5))
    safe_place(Prototype.SmallElectricPole, Position(x=x - 2, y=26.5))
    safe_place(Prototype.Lab, Position(x=x, y=24.5))
    safe_place(Prototype.Inserter, Position(x=x - 2, y=24.5), Direction.RIGHT)
print(inspect_inventory())
