library ieee;
use ieee.std_logic_1164.all;

use work.conf.all;


entity sea_battle is
	port(
		clk: in std_logic;

		i_buttons: in std_logic_vector(0 to BUTTON_COUNT - 1);
		i_noise: in std_logic;

		o_led_rows: out std_logic_vector(0 to FIELD_SIZE.y - 1);
		o_led_columns: out std_logic_vector(0 to FIELD_SIZE.x - 1);
		o_info_leds: out std_logic_vector(0 to 7)
	);
end entity;


architecture behavioral of sea_battle is
begin
	u_single_player: entity work.single_player
	port map (
		clk => clk,

		i_buttons => i_buttons,
		i_noise => i_noise,

		o_led_rows => o_led_rows,
		o_led_columns => o_led_columns,
		o_info_leds => o_info_leds
	);
end architecture;
