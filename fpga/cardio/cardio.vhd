library ieee ;
use ieee.std_logic_1164.all ;

entity cardio is
  port
	(-- Input ports
    clk       : in  std_logic ;
    heart     : in  std_logic ;
    -- Output ports
	 led_0     : buffer std_logic ;
	 led_1     : buffer std_logic ;
	 led_2     : buffer std_logic ) ;
end entity cardio ;

architecture cardio_arch of cardio is
  signal cnt       : integer range 0 to 127 := 0 ;
  signal heart_old : std_logic := '0' ;
begin

	main : process( clk )
	begin
		if( rising_edge( clk ) ) then
			if( heart_old = '1' and heart = '0' ) then
				if( cnt > 90 ) then    -- too slow			
					led_0 <= '0' ;
					led_1 <= '0' ;
					led_2 <= '1' ;
				elsif( cnt < 24 ) then -- too fast
					led_0 <= '1' ;
					led_1 <= '0' ;
					led_2 <= '0' ;
				else                   -- good range
					led_0 <= '0' ;
					led_1 <= '1' ;
					led_2 <= '0' ;
				end if;
				cnt <= 0 ;             -- reset counter
			else
				if( cnt /= 127 ) then
					cnt <= cnt + 1 ;
				end if ;
			end if ;
			heart_old <= heart ;
		end if ;
	end process main ;

end architecture cardio_arch ;
