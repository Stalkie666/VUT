-- uart_rx_fsm.vhd: UART controller - finite state machine controlling RX side
-- Author(s): Jakub Hamadej (xhamad03)

library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_unsigned.all;
use ieee.numeric_std.all;



entity UART_RX_FSM is
    port(
       CLK : in std_logic;
       RST : in std_logic;
       DIN : in std_logic;
       CNTTO10 : in std_logic_vector(3 downto 0);
       DOUT_VLD : out std_logic;
       START_COUNTING : out std_logic
    );
end entity;



architecture behavioral of UART_RX_FSM is
    -- declaring States
    type t_state is (IDLE, READ_DATA,END_IDLE);
    signal STATE_TYPES : t_state := IDLE;
begin
    -- behavior of states
    state_logic : process (CLK,RST) begin
        if RST = '1' then
            STATE_TYPES <= IDLE;
        elsif rising_edge(CLK) then
            case STATE_TYPES is
                when IDLE =>
                    if DIN = '0' then
                        STATE_TYPES <= READ_DATA;
                    end if;
                when READ_DATA =>
                    if CNTTO10 = "1010" then
                        STATE_TYPES <= END_IDLE;
                    end if;
                when END_IDLE =>
                    STATE_TYPES <= IDLE;
                when others =>
                    STATE_TYPES <= IDLE;
            end case;
        end if;
    end process state_logic;

    -- bahevior of output for States
    output_logic : process(STATE_TYPES) begin
        case STATE_TYPES is
            when IDLE =>
                DOUT_VLD <= '0';
                START_COUNTING <= '0';
            when READ_DATA =>
                DOUT_VLD <= '0';
                START_COUNTING <= '1';
            when END_IDLE =>
                DOUT_VLD <= '1';
                START_COUNTING <= '0';
        end case;
    end process output_logic;

end architecture;
