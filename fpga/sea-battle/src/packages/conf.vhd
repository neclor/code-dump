library ieee;
use ieee.std_logic_1164.all;
use work.my_std.all;


package conf is
	constant FIELD_SIZE: vector2i := (7, 7);
	constant MAX_SHIPS: positive := 7;
	constant BUTTON_COUNT: positive := 4;
end package;
