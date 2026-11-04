# Lab entities can't be fetched once they hold packs (serializer bug), so feed it via chest + inserter.
craft_item(Prototype.Inserter, quantity=1)
craft_item(Prototype.IronChest, quantity=1)
go(Position(x=-9.5, y=29.5))
place_entity(Prototype.SmallElectricPole, position=Position(x=-11.5, y=24.5))
ins = place_entity(Prototype.Inserter, direction=Direction.UP, position=Position(x=-8.5, y=26.5))
place_entity(Prototype.SmallElectricPole, position=Position(x=-10.5, y=26.5))
chest = place_entity(Prototype.IronChest, position=ins.pickup_position)
chest = insert_item(Prototype.AutomationSciencePack, chest, quantity=inv(Prototype.AutomationSciencePack))
PACK_CHEST = chest.position
print('pack chest', PACK_CHEST, chest.inventory, 'inserter', ins.pickup_position, ins.drop_position)
