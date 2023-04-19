-- uart_rx.vhd: UART controller - receiving (RX) side
-- Author(s): Name Surname (xlogin00)

library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_unsigned.all;



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
    --vnejsi dratovani, mozna jeste upravim
    signal rstStart : std_logic := '0';
    signal cnt15 : std_logic := '0';
    signal cnt8 : std_logic := '0';
    signal dCnt10 : std_logic := '0';
    signal qCnt10 : std_logic := '0';
    signal endBit : std_logic := '0';
begin

    -- Instance of RX FSM
    fsm: entity work.UART_RX_FSM
    port map (
        CLK => CLK,
        RST => RST
    );

    DOUT <= (others => '0');
    DOUT_VLD <= '0';

    -- START_BIT
    startBit: process(CLK,RST,qCnt10) begin
    end process startBit;

    -- COUNTER15
    counter15: process(CLK, rstStart) begin
    end process counter15;
    
    --SHIFT_REGISTER
    shiftRegister: process(cnt8,RST) begin
    end process shiftRegister;

    -- COUNTER10
    counter10: process(cnt15, rstStart) begin
    end process counter10;

    -- DELAY
    delay: process(CLK,RST) begin
    end process delay;

    -- END_BIT_TEST
    endBitTest: process() begin
    end process endBitTest;

end architecture;
