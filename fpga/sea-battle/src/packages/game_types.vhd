library ieee;
use ieee.std_logic_1164.all;
use work.boolean_vector_pkg.all;
use work.game_config.all;


package game_types is
	attribute syn_encoding : string;
	type game_state is (SETUP, TURN_1, TURN_2, GAME_OVER);
	attribute syn_encoding of game_state : type is "sequential";

	subtype boolean_field is boolean_vector(0 to FIELD_LENGTH - 1);

	subtype position is boolean_field;
end package;
