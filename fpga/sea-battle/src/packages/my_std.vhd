library ieee;
use ieee.std_logic_1164.all;


package my_std is
	type vector2i is record
		x: integer;
		y: integer;
	end record;

	--type boolean_vector is array (natural range <>) of boolean;
	type boolean_matrix is array (natural range <>, natural range <>) of boolean;

	function to_std_logic(bool: boolean) return std_logic;
	function to_boolean(logic: std_logic) return boolean;

	function to_std_logic_vector(bool_vector: boolean_vector) return std_logic_vector;
	function to_boolean_vector(logic_vector: std_logic_vector) return boolean_vector;

	function get_row(matrix: boolean_matrix; y: integer) return boolean_vector;

	function max(left: integer; right: integer) return integer;
end package;


package body my_std is
	function to_std_logic(bool: boolean) return std_logic is
	begin
		if bool then
			return '1';
		else
			return '0';
		end if;
	end function;

	function to_boolean(logic: std_logic) return boolean is
	begin
		return logic = '1';
	end function;

	function to_std_logic_vector(bool_vector: boolean_vector) return std_logic_vector is
		variable logic_vector: std_logic_vector(bool_vector'range);
	begin
		for i in bool_vector'range loop
			logic_vector(i) := to_std_logic(bool_vector(i));
		end loop;
		return logic_vector;
	end function;

	function to_boolean_vector(logic_vector: std_logic_vector) return boolean_vector is
		variable bool_vector: boolean_vector(logic_vector'range);
	begin
		for i in logic_vector'range loop
			bool_vector(i) := to_boolean(logic_vector(i));
		end loop;
		return bool_vector;
	end function;

	function get_row(matrix: boolean_matrix; y: integer) return boolean_vector is
		variable bool_vector: boolean_vector(matrix'range(2));
	begin
		for x in matrix'range(2) loop
			bool_vector(x) := matrix(y, x);
		end loop;
    	return bool_vector;
	end function;

	function max(left: integer; right: integer) return integer is
	begin
		if left > right then
			return left;
		else
			return right;
		end if;
	end function;
end package body;
