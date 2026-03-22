library ieee;
use ieee.std_logic_1164.all;
use work.my_std.all;

use work.conf.all;
use work.game_types.all;


entity single_player is
	port(
		clk: in std_logic;

		i_buttons: in std_logic_vector(0 to BUTTON_COUNT - 1);
		i_noise: in std_logic;

		o_led_rows: out std_logic_vector(0 to FIELD_SIZE.y - 1);
		o_led_columns: out std_logic_vector(0 to FIELD_SIZE.x - 1);
		o_info_leds: out std_logic_vector(0 to 7)
	);
end entity;


architecture behavioral of single_player is
	constant MAP_MODE_COUNT: integer := 6;

	signal map_mode: integer range 0 to MAP_MODE_COUNT - 1 := 2; -- 0 - DESTROYED, 1 - SHOTTED, 2 - SHIPS, 3 - ENEMY_DESTROYED, 4 - ENEMY_SHOTED, 5 - ENEMY_SHIPS -- debug
	signal blinker_state: boolean;

	signal rng_random: integer range 0 to max(FIELD_SIZE.y, FIELD_SIZE.x) - 1;
	signal old_random: integer range 0 to max(FIELD_SIZE.y, FIELD_SIZE.x) - 1;

	signal buttons: boolean_vector(0 to BUTTON_COUNT - 1);
	signal led_data: boolean_matrix(0 to FIELD_SIZE.y - 1, 0 to FIELD_SIZE.x - 1);

	signal inpts: input_vector(0 to 1);
	signal plrs: player_vector(0 to 1);

	signal state: game_state;

	signal dbg: boolean := false;

	procedure move_cursor(
		signal buttons: in boolean_vector(0 to BUTTON_COUNT - 1);
		signal inpt: inout input
	) is
	begin
		if buttons(2) then
			if inpt.pos.x = 0 then
				inpt.pos.x <= FIELD_SIZE.x - 1;
				if inpt.pos.y = 0 then
					inpt.pos.y <= FIELD_SIZE.y - 1;
				else
					inpt.pos.y <= inpt.pos.y - 1;
				end if;

			else
				inpt.pos.x <= inpt.pos.x - 1;
			end if;

		elsif buttons(3) then
			if inpt.pos.x = FIELD_SIZE.x - 1 then
				inpt.pos.x <= 0;
				if inpt.pos.y = FIELD_SIZE.y - 1 then
					inpt.pos.y <= 0;
				else
					inpt.pos.y <= inpt.pos.y + 1;
				end if;

			else
				inpt.pos.x <= inpt.pos.x + 1;
			end if;
		end if;
	end procedure;

	procedure draw_map(
		signal player_field: in field_t;
		signal ai_field: in field_t;
		signal map_mode: in integer range 0 to MAP_MODE_COUNT - 1;

		signal player_input: in input;
		signal blinker_state: in boolean;

		signal led_data: out boolean_matrix(0 to FIELD_SIZE.y - 1, 0 to FIELD_SIZE.x - 1);
		signal info_leds: out std_logic_vector(0 to 7)
	) is
	begin
		led_data <= (others => (others => false));
		for x in led_data'range(1) loop
			for y in led_data'range(2) loop
				case map_mode is -- 0 - DESTROYED, 1 - SHOTTED, 2 - SHIPS, 3 - ENEMY_DESTROYED, 4 - ENEMY_SHOTED, 5 - ENEMY_SHIPS
					when 0 | 1 =>
						led_data(x, y) <= (ai_field(x, y) = DESTROYED) or (map_mode = 1 and ai_field(x, y) = MISSED);

					when 2 =>
						led_data(x, y) <= player_field(x, y) = SHIP;

					when 3 | 4 =>
						led_data(x, y) <= (player_field(x, y) = DESTROYED) or (map_mode = 4 and player_field(x, y) = MISSED);

					when 5 =>
						led_data(x, y) <= ai_field(x, y) = SHIP;
				end case;
			end loop;
		end loop;

		led_data(player_input.pos.y, player_input.pos.x) <= blinker_state;

		info_leds(0 to MAP_MODE_COUNT - 1) <= (others => '0');
		info_leds(map_mode) <= '1';
	end procedure;
begin

	u_blinker: entity work.blinker
	generic map (
		PERIOD => 500
	)
	port map (
		clk => clk,
		state => blinker_state
	);

	u_rng: entity work.rng
	generic map (
		MAX_VALUE => max(FIELD_SIZE.y, FIELD_SIZE.x) - 1
	)
	port map (
		clk => clk,
		noise => i_noise,
		random => rng_random
	);

	u_button_handler: entity work.button_handler
	generic map (
		BUTTON_COUNT => BUTTON_COUNT
	)
	port map (
		clk => clk,
		i_buttons => i_buttons,
		o_buttons => buttons
	);

	u_leds: entity work.led_matrix_controller
	generic map (
		SIZE => FIELD_SIZE
	)
	port map (
		clk => clk,
		data => led_data,
		o_rows => o_led_rows,
		o_columns => o_led_columns
	);

	u_logic: entity work.logic
	port map (
		clk => clk,
		inputs => inpts,
		players => plrs,
		state => state
	);

	main: process(clk)
		variable random_pos: coordinate;
	begin
		if (rising_edge(clk)) then
			o_info_leds(6 to 7) <= (others => '0');

			random_pos := (old_random mod FIELD_SIZE.y, rng_random mod FIELD_SIZE.x);
			inpts(1).pos <= random_pos;
			inpts(1).press <= false;

			move_cursor(buttons, inpts(0));
			if buttons(0) then
				if map_mode = MAP_MODE_COUNT - 1 then
					map_mode <= 0;
				else
					map_mode <= map_mode + 1;
				end if;
			end if;
			inpts(0).press <= buttons(1);

			case state is
				when SETUP =>
					inpts(1).press <= true;

				when TURN =>
					if plrs(0).field(random_pos.y, random_pos.x) = EMPTY or plrs(0).field(random_pos.y, random_pos.x) = SHIP then
						inpts(1).press <= true;
					end if;

				when GAME_OVER =>
					if plrs(0).ships = 0 then
						o_info_leds(7) <= '1';
					else
						o_info_leds(6) <= '1';
					end if;

				when others =>
					null;
			end case;

			draw_map(plrs(0).field, plrs(1).field, map_mode, inpts(0), blinker_state, led_data, o_info_leds);
			old_random <= rng_random;
		end if;
	end process;

end architecture;
