# FLE's move_to builds paths from a cached player_location that can drift from the real character
# (only successful move_to calls refresh it; connect_entities/placing while walking do not).
# go() re-reads the real position first. Use go() instead of move_to() everywhere.
def sync_location():
    out = move_to.connection.rcon_client.send_command(
        "/sc local c=storage.agent_characters[1] rcon.print(c.position.x..','..c.position.y)")
    x, y = [float(v) for v in out.split(',')]
    move_to.game_state.player_location = Position(x=x, y=y)
    return Position(x=x, y=y)

def go(pos):
    sync_location()
    return move_to(pos)

def inv(p):
    return inspect_inventory()[p]

def gather(resource, qty):
    pos = nearest(resource)
    go(pos)
    return harvest_resource(pos, quantity=qty)

def near(pos, dy=-2):
    here = sync_location()
    if pos.distance(here) > 8:
        move_to(Position(x=pos.x, y=pos.y + dy))

def collect_plates():
    total = 0
    for f in get_entities({Prototype.StoneFurnace}):
        for proto in (Prototype.IronPlate, Prototype.CopperPlate):
            try:
                n = f.furnace_result[proto]
                if n:
                    near(f.position)
                    total += extract_item(proto, f, quantity=n)
            except Exception:
                pass
    return total

print('go/gather/near/collect_plates defined; at', sync_location())
