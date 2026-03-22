library ieee;
use ieee.std_logic_1164.all;
use work.conf.all;


package game_types is
	type game_state is (SETUP, TURN, GAME_OVER);

	type cell is (EMPTY, SHIP, DESTROYED, MISSED);
	type field_t is array (0 to FIELD_SIZE.y - 1, 0 to FIELD_SIZE.x - 1) of cell;

	type coordinate is record
		x: integer range 0 to FIELD_SIZE.x - 1;
		y: integer range 0 to FIELD_SIZE.y - 1;
	end record;

	type input is record
		press: boolean;
		pos: coordinate;
	end record;
	type input_vector is array (natural range <>) of input;

	type player is record
    	field: field_t;
    	ships: integer range 0 to MAX_SHIPS;
	end record;
	type player_vector is array (natural range <>) of player;
end package;
