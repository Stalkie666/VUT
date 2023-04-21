-- uart_rx.vhd: UART controller - receiving (RX) side
-- Author(s): Name Surname (xlogin00)

library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_unsigned.all;
use ieee.std_logic_arith.all;
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
    --vnejsi dratovani, mozna jeste upravim
    signal rstStart : std_logic := '0'; --vystup ze startBit
    signal cnt15 : std_logic := '0'; -- vystup ze cnt15 pro cnt10
    signal cnt8 : std_logic := '0'; -- vystup ze cnt15 pro shift register
    signal dCnt10 : std_logic := '0'; -- vystup z cnt10 do delay
    signal qCnt10 : std_logic := '0'; -- vystup z delay do rst startBit a na DOUT_VLD
    signal endBit : std_logic := '0'; -- vystup z shift registru do DOUT_VLD skrz and spolecne s qcnt10
    --vnitrni signaly jednotlivych procesu
    signal count15 : std_logic_vector (3 downto 0) := "0000";
    signal reg9 : std_logic_vector(8 downto 0) := "000000000";
    signal count10 : std_logic_vector(3 downto 0) := "0000";
begin

    -- Instance of RX FSM
    fsm: entity work.UART_RX_FSM
    port map (
        CLK => CLK,
        RST => RST,
        DIN => DIN,
        DOUT_VLD => DOUT_VLD,
        DCNT10 => dCnt10,
        QCNT10 => qcnt10,
        RSTSTART => rstStart
    );

    -- START_BIT
    startBit: process(CLK,RST,qCnt10) begin
        if RST = '1' or qCnt10 = '1' then
            rstStart <= '0';
        elsif rising_edge(CLK) then
            if DIN = '1' and rstStart = '0' then
                rstStart <= '1';
            else
                rstStart <= '0';
            end if;
        end if;
    end process startBit;

    -- COUNTER15
    counter15: process(CLK, rstStart) begin
        if rstStart = '0' then
            count15 <= "0000";
            cnt8 <= '0';
            cnt15 <= '0';
        elsif rising_edge(CLK) then
            if count15 = "0111" then
                cnt8 <= '1';
                cnt15 <= '0';
            elsif count15 = "1111" then
                cnt8 <= '0';
                cnt15 <= '1';
            else
                cnt8 <= '0';
                cnt15 <= '1';
            end if;
            count15 <= std_logic_vector( unsignef(count15) + 1 );
        end if;
    end process counter15;
    
    --SHIFT_REGISTER
    shiftRegister: process(cnt8,RST) begin
        if RST = '1' then
            reg9 <= (others => '0');
        elsif rising_edge(cnt8) then
            reg9 <= DIN & reg9(8 downto 1);
        end if;
        endBit <= reg9(8);
    end process shiftRegister;

    -- COUNTER10
    counter10: process(cnt15, rstStart) begin
        if rstStart = '0' then
            count10 <= "0000";
            dCnt10 <= '0';
        elsif rising_edge(cnt15) then
            if count10 = "1001" then
                dCnt10 <= '1';
            else
                dCnt10 <= '0';
            end if;
            count10 <= std_logic_vector( unsignef(count10) + 1);
        end if;
    end process counter10;

    -- DELAY
    delay: process(CLK,RST) begin
        if RST = '1' then
            qCnt10 <= '0';
        elsif rising_edge(CLK) then
            qCnt10 <= dCnt10;
        end if;
    end process delay;

    DOUT <= reg9(8 downto 1);
    DOUT_VLD <= qCnt10 and endBit;

end architecture;
