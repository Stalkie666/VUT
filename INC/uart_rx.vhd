-- uart_rx.vhd: UART controller - receiving (RX) side
-- Author(s): Name Surname (xlogin00)

library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_unsigned.all;
use ieee.numeric_std.all;



-- Entity declaration (DO NOT ALTER THIS PART!)
entity UART_RX is
    port(
        CLK      : in std_logic;
        RST      : in std_logic;
        DIN      : in std_logic;
        DOUT     : out std_logic_vector(7 downto 0);
        DOUT_VLD : out std_logic
    );
end entity;



-- Architecture implementation (INSERT YOUR IMPLEMENTATION HERE)
architecture behavioral of UART_RX is
    signal cntTo15 : std_logic_vector(3 downto 0);
    signal all10Bits : std_logic_vector (9 downto 0);
    signal vld_out : std_logic;
    signal cntTo10 : std_logic_vector(3 downto 0);
    signal start_counting : std_logic;
begin

    -- Instance of RX FSM
    fsm: entity work.UART_RX_FSM
    port map (
        CLK => CLK,
        RST => RST,
        DOUT_VLD => vld_out,
        DIN => DIN,
        CNTTO10 => cntTo10,
        START_COUNTING => start_counting
    );

    --DOUT <= (others => '0');
    DOUT_VLD <= vld_out;

    counting15 : process(CLK,start_counting) begin
        if start_counting = '0' then
            cntTo15 <= "0000";
        elsif rising_edge(CLK) then
            cntTo15 <= cntTo15 + 1;
        end if;
    end process counting15;

    counting10 : process(CLK,start_counting) begin
        if start_counting = '0' then 
            cntTo10 <= "0000";
        elsif rising_edge(CLK) and cntTo15 = "1111" then
            cntTo10 <= cntTo10 + 1;
        end if;
    end process counting10;

    chooseReg: process(CLK,RST,DIN) begin
        if RST = '1' then
            DOUT <= (others => '0');
        elsif rising_edge(CLK) and cntTo15 = "1000" then
            case cntTo10 is
                when "0001" => DOUT(0) <= DIN;
                when "0010" => DOUT(1) <= DIN;
                when "0011" => DOUT(2) <= DIN;
                when "0100" => DOUT(3) <= DIN;
                when "0101" => DOUT(4) <= DIN;
                when "0110" => DOUT(5) <= DIN;
                when "0111" => DOUT(6) <= DIN;
                when "1000" => DOUT(7) <= DIN;
                when others => null;
            end case;
        end if;
    end process chooseReg;

end architecture;
