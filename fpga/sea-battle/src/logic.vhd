library ieee;
use ieee.std_logic_1164.all;
use work.my_std.all;

use work.conf.all;
use work.game_types.all;


entity logic is
	port(
		clk: in std_logic;

		inputs: in input_vector(0 to 1);
		players: out player_vector(0 to 1);
		state: out game_state
	);
end entity;


architecture behavioral of logic is
	signal s_state: game_state := SETUP;

	signal plrs: player_vector(0 to 1) := (
		(field => (others => (others => EMPTY)), ships => 0),
		(field => (others => (others => EMPTY)), ships => 0)
	);
	signal old_pressds: boolean_vector(0 to 1) := (others => false);

	signal a: integer range 0 to 1 := 0;
begin

	players <= plrs;
	state <= s_state;

	main: process(clk)
		variable b: integer range 0 to 1;
	begin
		if rising_edge(clk) then
			b := 1 - a;
			for i in inputs'range loop
				old_pressds(i) <= inputs(i).press;
			end loop;

			case s_state is
				when SETUP =>
					if inputs(a).press or old_pressds(a) then
						if plrs(a).ships < MAX_SHIPS then
							if plrs(a).field(inputs(a).pos.y, inputs(a).pos.x) = EMPTY then
								plrs(a).ships <= plrs(a).ships + 1;
							end if;
							plrs(a).field(inputs(a).pos.y, inputs(a).pos.x) <= SHIP;
						end if;
					end if;

					if plrs(a).ships = MAX_SHIPS and plrs(b).ships = MAX_SHIPS then
						s_state <= TURN;
					end if;
					a <= b;

				when TURN =>
					if inputs(a).press then
						case plrs(b).field(inputs(a).pos.y, inputs(a).pos.x) is
							when SHIP =>
								plrs(b).field(inputs(a).pos.y, inputs(a).pos.x) <= DESTROYED;
								plrs(b).ships <= plrs(b).ships - 1;
								if plrs(b).ships - 1 = 0 then
									s_state <= GAME_OVER;
								end if;

							when EMPTY =>
								plrs(b).field(inputs(a).pos.y, inputs(a).pos.x) <= MISSED;

							when others =>
								null;
						end case;
						a <= b;
					end if;

				when others =>
					null;
			end case;
			end if;
	end process;
end architecture;
