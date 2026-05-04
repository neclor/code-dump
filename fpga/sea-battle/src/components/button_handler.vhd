library ieee;
use ieee.std_logic_1164.all;
use work.boolean_vector_pkg.all;


entity button_handler is
	generic(
		CLK_FREQ : positive := 320; -- Hz
		PERIOD : natural := 32; -- ms
		BUTTON_MAX : positive := 1
	);
	port(
		clk : in std_logic;

		i_buttons : in std_logic_vector(0 to BUTTON_MAX - 1);
		o_buttons : out boolean_vector(0 to BUTTON_MAX - 1)
	);
end entity;


architecture behavioral of button_handler is
	constant BUTTON_PRESSED : std_logic := '0';
    constant BUTTON_RELEASED : std_logic := '1';

	constant TICKS: natural := CLK_FREQ * PERIOD / 1000 ;
	signal tick_count: natural range 0 to TICKS := 0;

	signal buttons_old : std_logic_vector(0 to BUTTON_MAX - 1) := (others => BUTTON_RELEASED);
begin

	main : process(clk)
	begin
		if rising_edge(clk) then
			o_buttons <= (others => false);

			if tick_count < TICKS then
				tick_count <= tick_count + 1;
			else
				tick_count <= 0;

				for i in i_buttons'range loop
					o_buttons(i) <= (i_buttons(i) = BUTTON_PRESSED) and (buttons_old(i) = BUTTON_RELEASED);
				end loop;
				buttons_old <= i_buttons;
			end if;
		end if;
	end process;
end architecture;
