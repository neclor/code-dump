library ieee;
use ieee.std_logic_1164.all;


package game_config is
	constant FIELD_SIZE_X : natural := 3;
	constant FIELD_SIZE_Y : natural := 3;
	constant FIELD_LENGTH : positive := FIELD_SIZE_X * FIELD_SIZE_Y;


	constant BUTTON_1_LEFT : natural := 2;
	constant BUTTON_1_RIGHT : natural := 1;
	constant BUTTON_1_SHOOT : natural := 0;

	constant BUTTON_2_LEFT : natural := 3;
	constant BUTTON_2_RIGHT : natural := 4;
	constant BUTTON_2_SHOOT : natural := 5;

	constant BUTTON_MAX : positive := 6;


	constant LED_TURN_1 : natural := 0;
	constant LED_WIN_1 : natural := 1;

	constant LED_TURN_2 : natural := 2;
	constant LED_WIN_2 : natural := 3;

	constant LED_MAX : positive := 4;
end package;
