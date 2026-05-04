library ieee;
use ieee.std_logic_1164.all;

use work.boolean_vector_pkg.all;
use work.game_config.all;
use work.game_types.all;


entity sea_battle is
	port(
		clk : in std_logic;

		i_reset : in std_logic;
		i_buttons : in std_logic_vector(0 to BUTTON_MAX - 1);

		o_led_rows_1 : out std_logic_vector(0 to FIELD_SIZE_Y - 1);
		o_led_columns_1_green : out std_logic_vector(0 to FIELD_SIZE_X - 1);
		o_led_columns_1_red : out std_logic_vector(0 to FIELD_SIZE_X - 1);

		o_led_rows_2 : out std_logic_vector(0 to FIELD_SIZE_Y - 1);
		o_led_columns_2_green : out std_logic_vector(0 to FIELD_SIZE_X - 1);
		o_led_columns_2_red : out std_logic_vector(0 to FIELD_SIZE_X - 1);

		o_info_leds : out std_logic_vector(0 to LED_MAX - 1)
	);
end entity;


architecture behavioral of sea_battle is
	signal buttons : boolean_vector(0 to BUTTON_MAX - 1);

	signal led_data_1_green : boolean_vector(0 to FIELD_LENGTH - 1);
	signal led_data_1_red : boolean_vector(0 to FIELD_LENGTH - 1);

	signal led_data_2_green : boolean_vector(0 to FIELD_LENGTH - 1);
	signal led_data_2_red : boolean_vector(0 to FIELD_LENGTH - 1);

	signal position_1 : position := (0 => true, others => false);
	signal position_2 : position := (0 => true, others => false);

	signal ships_1 : boolean_field := (others => false);
	signal ships_2 : boolean_field := (others => false);

	signal shots_1 : boolean_field := (others => false);
	signal shots_2 : boolean_field := (others => false);

	signal has_ship_1 : boolean := false;
	signal has_ship_2 : boolean := false;

	signal state : game_state := SETUP;


	procedure move_cursor(
		button_left : in boolean;
		button_right : in boolean;
		signal position : inout position
	) is
	begin
		if button_left then
			position <= position(1 to FIELD_LENGTH - 1) & position(0);
		end if;
		if button_right then
			position <= position(FIELD_LENGTH - 1) & position(0 to FIELD_LENGTH - 2);
		end if;
	end procedure;


	procedure place_ship(
		pressed : in boolean;
		position : in position;
		signal ships : inout boolean_field;
		signal has_ship : inout boolean
	) is
	begin
		if pressed and not has_ship then
			for i in position'range loop
				if position(i) then
					ships(i) <= true;
					has_ship <= true;
				end if;
			end loop;
		end if;
	end procedure;


	procedure shoot(
		position : in position;
		signal shots : inout boolean_field;
		target_ships : in boolean_field;
		signal target_has_ship : inout boolean;
		signal game_state : inout game_state
	) is
	begin
		for i in position'range loop
			if position(i) and not shots(i) and target_ships(i) then
				target_has_ship <= false;
				game_state <= GAME_OVER;
			end if;
			shots(i) <= shots(i) or position(i);
		end loop;
	end procedure;
begin

	u_button_handler: entity work.button_handler
	generic map (
		BUTTON_MAX => BUTTON_MAX
	)
	port map (
		clk => clk,
		i_buttons => i_buttons,
		o_buttons => buttons
	);

	u_leds_1: entity work.bicolor_led_matrix_controller
	generic map (
		LENGTH => FIELD_LENGTH,
		SIZE_X => FIELD_SIZE_X,
		SIZE_Y => FIELD_SIZE_Y
	)
	port map (
		clk => clk,
		data_green => led_data_1_green,
		data_red => led_data_1_red,
		o_rows => o_led_rows_1,
		o_columns_green => o_led_columns_1_green,
		o_columns_red => o_led_columns_1_red
	);

	u_leds_2: entity work.bicolor_led_matrix_controller
	generic map (
		LENGTH => FIELD_LENGTH,
		SIZE_X => FIELD_SIZE_X,
		SIZE_Y => FIELD_SIZE_Y
	)
	port map (
		clk => clk,
		data_green => led_data_2_green,
		data_red => led_data_2_red,
		o_rows => o_led_rows_2,
		o_columns_green => o_led_columns_2_green,
		o_columns_red => o_led_columns_2_red
	);


	main : process(clk)
		alias pressed_1 : boolean is buttons(BUTTON_1_SHOOT);
		alias pressed_2 : boolean is buttons(BUTTON_2_SHOOT);

		alias left_1 : boolean is buttons(BUTTON_1_LEFT);
		alias left_2 : boolean is buttons(BUTTON_2_LEFT);

		alias right_1 : boolean is buttons(BUTTON_1_RIGHT);
		alias right_2 : boolean is buttons(BUTTON_2_RIGHT);
	begin
		if (rising_edge(clk)) then

			if i_reset = '0' then
				state <= SETUP;
				position_1 <= (0 => true, others => false);
				position_2 <= (0 => true, others => false);
				ships_1 <= (others => false);
				ships_2 <= (others => false);
				shots_1 <= (others => false);
				shots_2 <= (others => false);
				has_ship_1 <= false;
				has_ship_2 <= false;
			else
				move_cursor(left_1, right_1, position_1);
				move_cursor(left_2, right_2, position_2);

				case state is
					when SETUP =>
						place_ship(pressed_1, position_1, ships_1, has_ship_1);
						place_ship(pressed_2, position_2, ships_2, has_ship_2);
						if has_ship_1 and has_ship_2 then
							state <= TURN_1;
						end if;

					when TURN_1 =>
						if pressed_1 then
							state <= TURN_2;
							shoot(position_1, shots_1, ships_2, has_ship_2, state);
						end if;

					when TURN_2 =>
						if pressed_2 then
							state <= TURN_1;
							shoot(position_2, shots_2, ships_1, has_ship_1, state);
						end if;

					when others => null;
				end case;
			end if;
		end if;
	end process;

	draw_map_1 : process(ships_1, shots_1, position_1, state)
	begin
		for i in led_data_1_red'range loop
			led_data_1_red(i) <= ((state = SETUP and ships_1(i)) or shots_1(i));

			led_data_1_green(i) <= position_1(i);
		end loop;
	end process;

	draw_map_2 : process(ships_2, shots_2, position_2, state)
	begin
		for i in led_data_2_red'range loop
			led_data_2_red(i) <= ((state = SETUP and ships_2(i)) or shots_2(i));

			led_data_2_green(i) <= position_2(i);
		end loop;
	end process;


	o_info_leds(LED_TURN_1) <= '1' when state = TURN_1 else '0';
	o_info_leds(LED_TURN_2) <= '1' when state = TURN_2 else '0';

	o_info_leds(LED_WIN_1) <= '1' when state = GAME_OVER and not has_ship_2 else '0';
	o_info_leds(LED_WIN_2) <= '1' when state = GAME_OVER and not has_ship_1 else '0';

end architecture;
