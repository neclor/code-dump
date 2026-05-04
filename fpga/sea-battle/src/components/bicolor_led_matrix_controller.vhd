library ieee;
use ieee.std_logic_1164.all;
use work.boolean_vector_pkg.all;


entity bicolor_led_matrix_controller is
	generic(
		LENGTH : positive := 16;
		SIZE_X : positive := 4;
		SIZE_Y : positive := 4
	);
	port(
		clk : in std_logic;

		data_green : in boolean_vector(0 to LENGTH - 1);
		data_red : in boolean_vector(0 to LENGTH - 1);

		o_rows : out std_logic_vector(0 to SIZE_Y - 1); -- (-)
		o_columns_green : out std_logic_vector(0 to SIZE_X - 1); -- (+)
		o_columns_red : out std_logic_vector(0 to SIZE_X - 1) -- (+)
	);
end entity;


architecture behavioral of bicolor_led_matrix_controller is
	signal current_row : std_logic_vector(0 to SIZE_Y - 1) := (0 => '0', others => '1');
begin

	main : process(clk)
	begin
		if rising_edge(clk) then
			o_rows <= current_row;
			for i in 0 to SIZE_Y - 1 loop
				if current_row(i) = '0' then
					o_columns_green <= to_std_logic_vector(data_green(i * SIZE_X to (i + 1) * SIZE_X - 1));
					o_columns_red <= to_std_logic_vector(data_red(i * SIZE_X to (i + 1) * SIZE_X - 1));
				end if;
			end loop;
			current_row <= current_row(SIZE_Y - 1) & current_row(0 to SIZE_Y - 2);
		end if;
	end process;
end architecture;
