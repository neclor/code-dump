library ieee;
use ieee.std_logic_1164.all;


entity blinker is
	generic(
		CLK_FREQ: positive := 320; -- Hz
		PERIOD: positive := 1000 -- ms
	);
	port(
		clk: in std_logic;

		state: out boolean;
		tick: out boolean
	);
end entity;


architecture behavioral of blinker is
	constant TICKS: integer := CLK_FREQ * PERIOD / 2 / 1000 ;
	signal tick_count: integer range 0 to TICKS := 0;

	signal s_state: boolean := false;
begin

	state <= s_state;

	main: process(clk)
	begin
		if rising_edge(clk) then
			tick <= false;
			if tick_count < TICKS then
				tick_count <= tick_count + 1;
			else
				tick_count <= 0;

				tick <= not s_state;
				s_state <= not s_state;
			end if;
		end if;
	end process;
end architecture;
