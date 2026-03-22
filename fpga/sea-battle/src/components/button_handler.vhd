library ieee;
use ieee.std_logic_1164.all;
use work.my_std.all;


entity button_handler is
	generic(
		CLK_FREQ: positive := 320; -- Hz
		SCAN_PERIOD: natural := 100; -- ms
		BUTTON_COUNT: positive := 1
	);
	port(
		clk: in std_logic;

		i_buttons: in std_logic_vector(0 to BUTTON_COUNT - 1);
		o_buttons: out boolean_vector(0 to BUTTON_COUNT - 1)
	);
end entity;


architecture behavioral of button_handler is
	signal buttons_old: boolean_vector(0 to BUTTON_COUNT - 1) := (others => true);

	signal tick: boolean := false;
begin

	u_blinker: entity work.blinker
	generic map (
		CLK_FREQ => CLK_FREQ,
		PERIOD => SCAN_PERIOD
	)
	port map (
		clk => clk,
		tick => tick
	);

	main: process(clk)
	begin
		if rising_edge(clk) then
			o_buttons <= (others => false);
			if tick then
				for i in 0 to BUTTON_COUNT - 1 loop
					if i_buttons(i) = '0' and buttons_old(i) then
						o_buttons(i) <= true;
					end if;
				end loop;
				buttons_old <= to_boolean_vector(i_buttons);
			end if;
		end if;
	end process;
end architecture;
