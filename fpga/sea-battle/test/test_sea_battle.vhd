library ieee;
use ieee.std_logic_1164.all;
use work.boolean_vector_pkg.all;

use work.game_config.all;
use work.game_types.all;


entity test_sea_battle is
end entity;


architecture sim of test_sea_battle is
	signal clk : std_logic := '0';

	signal i_reset : std_logic := '1';
	signal i_buttons : std_logic_vector(0 to BUTTON_MAX - 1) := (others => '1');

	signal o_led_rows_1 : std_logic_vector(0 to FIELD_SIZE_Y - 1);
	signal o_led_rows_2 : std_logic_vector(0 to FIELD_SIZE_Y - 1);

	signal o_led_columns_green_1 : std_logic_vector(0 to FIELD_SIZE_X - 1);
	signal o_led_columns_red_1 : std_logic_vector(0 to FIELD_SIZE_X - 1);

	signal o_led_columns_green_2 : std_logic_vector(0 to FIELD_SIZE_X - 1);
	signal o_led_columns_red_2 : std_logic_vector(0 to FIELD_SIZE_X - 1);

	signal o_info_leds : std_logic_vector(0 to 7);
begin

	u_sea_battle: entity work.sea_battle
	port map (
		clk => clk,

		i_reset => i_reset,
		i_buttons => i_buttons,

		o_led_rows_1 => o_led_rows_1,
		o_led_rows_2 => o_led_rows_2,

		o_led_columns_1_green => o_led_columns_green_1,
		o_led_columns_1_red => o_led_columns_red_1,

		o_led_columns_2_green => o_led_columns_green_2,
		o_led_columns_2_red => o_led_columns_red_2,
		o_info_leds => o_info_leds
	);

	clk_process: process
	begin
		while now < 10 sec loop
			clk <= '0';
			wait for 1 ms;
			clk <= '1';
			wait for 1 ms;
		end loop;
		wait;
	end process;

	stim_proc: process
	begin
		i_buttons <= (others => '1');
		wait for 1 ms;

		-- place ship
		i_buttons(1) <= '0';
		wait for 50 ms;
		i_buttons(1) <= '1';
		wait for 50 ms;

		-- move left
		i_buttons(2) <= '0';
		wait for 50 ms;
		i_buttons(2) <= '1';
		wait for 50 ms;

		-- place ship
		i_buttons(1) <= '0';
		wait for 50 ms;
		i_buttons(1) <= '1';
		wait for 50 ms;

		-- move right
		i_buttons(3) <= '0';
		wait for 50 ms;
		i_buttons(3) <= '1';
		wait for 50 ms;

		-- shoot
		i_buttons(1) <= '0';
		wait for 50 ms;
		i_buttons(1) <= '1';
		wait for 50 ms;

		-- move right
		i_buttons(3) <= '0';
		wait for 50 ms;
		i_buttons(3) <= '1';
		wait for 50 ms;

		-- move right
		i_buttons(3) <= '0';
		wait for 50 ms;
		i_buttons(3) <= '1';
		wait for 50 ms;

		-- shoot
		i_buttons(1) <= '0';
		wait for 50 ms;
		i_buttons(1) <= '1';
		wait for 50 ms;

		wait;
	end process;
end architecture;
