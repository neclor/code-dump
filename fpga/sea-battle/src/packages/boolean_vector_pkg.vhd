library ieee;
use ieee.std_logic_1164.all;


package boolean_vector_pkg is
	type boolean_vector is array (natural range <>) of boolean;

	function to_std_logic(bool : boolean) return std_logic;

	function to_std_logic_vector(bool_vector : boolean_vector) return std_logic_vector;
end package;


package body boolean_vector_pkg is
	function to_std_logic(bool : boolean) return std_logic is
	begin
		if bool then
			return '1';
		else
			return '0';
		end if;
	end function;

	function to_std_logic_vector(bool_vector : boolean_vector) return std_logic_vector is
		variable logic_vector : std_logic_vector(bool_vector'range);
	begin
		for i in bool_vector'range loop
			logic_vector(i) := to_std_logic(bool_vector(i));
		end loop;
		return logic_vector;
	end function;
end package body;
