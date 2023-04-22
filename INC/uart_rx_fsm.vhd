-- uart_rx_fsm.vhd: UART controller - finite state machine controlling RX side
-- Author(s): Name Surname (xlogin00)

library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;



entity UART_RX_FSM is
    port(
        CLK : in std_logic;
        RST : in std_logic;
        DIN : in std_logic;
        DOUT_VLD : out std_logic;
        DCNT10 : in std_logic;
        QCNT10 : in std_logic;
    );
end entity;



architecture behavioral of UART_RX_FSM is
    type t_state is (IDLE,READ_DATA,SET_VALIDATE,END_IDLE);
    signal STATE_TYPES : t_state := IDLE;
    attribute fsm_encoding : string;
    attribute fsm_encoding of STATE_TYPES : signal is "sequential";
begin

    state_logic : process (CLK, RST) begin
        if RST = '1' then
            STATE_TYPES <= IDLE;
        elsif rising_edge(CLK) then
            case STATE_TYPES is
                -- cekani na 0 v DIN na zacatku
                when IDLE =>
                    if DIN = '0' then
                        STATE_TYPES <= READ_DATA;
                    end if;
                -- nacitani dat a endBitu 
                -- (ano nacte to i startBit ale ten se zahodi - vyhoda posuvneho registru)
                when READ_DATA =>
                    if DCNT10 = '1' then
                        STATE_TYPES <= SET_VALIDATE;
                    end if;
                -- Nastaveni validate na 1 clk
                when SET_VALIDATE =>
                    if QCNT10 = '1' then
                        STATE_TYPES <= END_IDLE;
                    end if;
                -- Preskoceni na cekani na dalsi sekvenci
                when END_IDLE =>
                    STATE_TYPES <= IDLE;
                -- Jen pro jistotu (nemelo by nikdy nastat)
                when others =>
                    STATE_TYPES <= IDLE;
            end case;
        end if;
    end process state_logic;

    output_logic : process (STATE_TYPES) begin
        case STATE_TYPES is
            when IDLE =>
                DOUT_VLD <= '0';
            when READ_DATA =>
                DOUT_VLD <= '0';
            when SET_VALIDATE =>
                DOUT_VLD <= '0';
            when END_IDLE =>
                DOUT_VLD <= '1';
        end case;
    end process output_logic;

end architecture;
