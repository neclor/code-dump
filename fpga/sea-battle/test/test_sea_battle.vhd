library ieee;
use ieee.std_logic_1164.all;
use work.my_std.all;

use work.conf.all;
use work.game_types.all;


entity test_sea_battle is
end entity;


architecture sim of test_sea_battle is
	signal clk: std_logic := '0';

	signal i_buttons: std_logic_vector(0 to BUTTON_COUNT - 1) := (others => '1');
	signal i_noise: std_logic := '0';

	signal o_led_rows: std_logic_vector(0 to FIELD_SIZE.y - 1);
	signal o_led_columns: std_logic_vector(0 to FIELD_SIZE.x - 1);
	signal o_info_leds: std_logic_vector(0 to 7);
begin

	u_sea_battle: entity work.sea_battle
	port map (
		clk => clk,

		i_buttons => i_buttons,
		i_noise => i_noise,

		o_led_rows => o_led_rows,
		o_led_columns => o_led_columns,
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
		i_noise <= '0';
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
