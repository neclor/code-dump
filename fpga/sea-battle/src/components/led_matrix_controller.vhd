library ieee;
use ieee.std_logic_1164.all;
use work.my_std.all;


entity led_matrix_controller is
	generic(
		CLK_FREQ: positive := 320; -- Hz
		SIZE: vector2i := (4, 4)
	);
	port(
		clk: in std_logic;

		data: in boolean_matrix(0 to SIZE.y - 1, 0 to SIZE.x - 1);

		o_rows: out std_logic_vector(0 to SIZE.y - 1); -- (-)
		o_columns: out std_logic_vector(0 to SIZE.x - 1) -- (+)
	);
end entity;


architecture behavioral of led_matrix_controller is
	signal y: integer range 0 to SIZE.x - 1 := 0;
begin

	main: process(clk)
	begin
		if rising_edge(clk) then
			o_rows <= (others => '1');
			o_rows(y) <= '0';

            o_columns <= to_std_logic_vector(get_row(data, y));

			if y = SIZE.x - 1 then
    			y <= 0;
			else
				y <= y + 1;
			end if;
		end if;
	end process;
end architecture;
