library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;
use ieee.math_real.all;
use work.my_std.all;


entity rng is
	generic(
		CLK_FREQ: positive := 320; -- Hz
		MAX_VALUE: integer := 255
	);
	port(
		clk: in std_logic;

		noise: in std_logic;
		random: out integer range 0 to MAX_VALUE
	);
end entity;


architecture behavioral of rng is
	constant BITS: integer := max(2, integer(ceil(log2(real(MAX_VALUE + 1)))));

	signal lfsr: std_logic_vector(BITS - 1 downto 0) := (others => '1');
begin

	main: process(clk)
		variable v_lsfr: std_logic_vector(BITS - 1 downto 0);
		variable feedback: std_logic;
	begin
		if rising_edge(clk) then
			feedback := lfsr(BITS - 1) xor lfsr(BITS - 2) xor noise;
			v_lsfr := lfsr(BITS - 2 downto 0) & feedback;
			lfsr <= v_lsfr;
			random <= to_integer(unsigned(v_lsfr)) mod (MAX_VALUE + 1);
		end if;
	end process;
end architecture;
