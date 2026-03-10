library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_unsigned.all;

entity test_cardio is
end;

architecture test_cardio_arc of test_cardio is
	signal heart			: std_logic;
	signal clk				: std_logic;
	signal led_slow		: std_logic;
	signal led_fast		: std_logic;
	constant clk_cycle	: integer := 1000;
	constant beat_cycle	: integer := 20;
	
	-- description of cardio
	component cardio
	port (
		heart			: in std_logic;
		clk 			: in std_logic;
		led_fast		: out std_logic;
		led_slow		: out std_logic);
		
	end component;
	
	-- beginning of the architecture port map
begin 

	DUT	:	cardio
		port map (
		heart			=> heart,
		led_slow		=> led_slow,
		led_fast		=> led_fast,
		clk			=> clk);
		
	-- Processes declaration
	clk_stimulus : process
	begin
		for i in 1 to clk_cycle loop
			clk <= '0';
			wait for 8 ms;
			clk <= '1';
			wait for 8 ms;
		end loop;
	end process clk_stimulus;	
		
		
	heart_stimulus : process
	begin
		for i in 1 to clk_cycle loop
			heart <= '0';
			wait for 500 ms;
			heart <= '1';
			wait for 500 ms;
		end loop;
		wait;
	end process heart_stimulus;	
	
end architecture test_cardio_arc;
	