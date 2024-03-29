----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date: 12/19/2022 11:28:28 AM
-- Design Name: 
-- Module Name: nn_controller - VHDL
-- Project Name: 
-- Target Devices: 
-- Tool Versions: 
-- Description: 
-- 
-- Dependencies: 
-- 
-- Revision:
-- Revision 0.01 - File Created
-- Additional Comments:
-- 
----------------------------------------------------------------------------------


library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

-- Uncomment the following library declaration if using
-- arithmetic functions with Signed or Unsigned values
use IEEE.NUMERIC_STD.ALL;

-- Uncomment the following library declaration if instantiating
-- any Xilinx leaf cells in this code.
--library UNISIM;
--use UNISIM.VComponents.all;

entity nn_controller is
    generic ( n_addr_bits : positive range 1 to 12 := 10);
    Port ( 
           clk              : in STD_LOGIC;
           rstb_busy        : in STD_LOGIC;
           ap_done          : in STD_LOGIC;
           ap_idle          : in STD_LOGIC;
           ap_ready         : in STD_LOGIC;
           rst_in           : in std_logic; 
           ctrl_reg         : in std_logic_vector (31 downto 0);    -- (start,done,
           ctrl_reg_write   : out std_logic_vector (31 downto 0);
           prediction       : in STD_LOGIC_VECTOR (31 downto 0);
           addr_in          : in STD_LOGIC_VECTOR (n_addr_bits-1 downto 0);
           addr_out         : out std_logic_vector (31 downto 0);
           bram_wr          : out std_logic_vector ( 3 downto 0);
           rst_out          : out std_logic;
           set_ap_start     : out STD_LOGIC;
           bram_en          : out std_logic;
           en_in            : in std_logic
           
           );
           
end nn_controller;

architecture VHDL of nn_controller is

--signal 
signal start_prev     : std_logic := '0';
signal bram_en_sig    : std_logic := '0';

begin
    
    process(start_prev,ctrl_reg,ap_done,ap_idle,clk,bram_en_sig)
    VARIABLE cnt : INTEGER := 0;
        begin
        if(cnt = 1) then
            bram_en_sig <= '0';
        end if;
        
        if rising_edge(clk) then
            if cnt < 3 then
                cnt := cnt + 1;
            end if;
        end if;
        
       
        if (cnt > 2 and rstb_busy = '0') then
            cnt := 0;
            bram_en_sig <= '1';
            if(start_prev = '0' and ap_idle = '1' and ctrl_reg(0) = '1') then
                        
                         bram_wr <= (others => '0');    -- read BRAM
                         addr_out(1 downto 0) <= "00";
                         addr_out(n_addr_bits+1 downto 2) <= addr_in;
                         addr_out(31 downto n_addr_bits+2) <=(others => '0');   
                                              
                         start_prev <= '1';
                         set_ap_start <= '1';            

                     
            elsif(ap_done = '0' and start_prev = '1') then
                            
                         bram_wr <= (others => '0');    -- read BRAM
                         addr_out(1 downto 0) <= "00";
                         addr_out(n_addr_bits+1 downto 2) <= addr_in;
                         addr_out(31 downto n_addr_bits+2) <=(others => '0'); 
        
            elsif(ap_done = '1' and start_prev = '1') then
                      
                         addr_out(31 downto 16) <= (others => '0');   -- ctrl addr
                         addr_out(15 downto 0) <= (others => '1');   -- ctrl addr
                         addr_out(1 downto 0) <= (others => '0');   -- ctrl addr                                           
                         bram_wr <= (others => '1'); 
                         start_prev <= '0';
                         ctrl_reg_write(31 downto 16) <=  prediction(15 downto 0);
                         ------------------------------- start---- done ---- idle ---- not used
                         ctrl_reg_write(15 downto 0) <= (0 => '0' ,1 => '1', 2 => '0',others => '0');
                         set_ap_start <= '0'; 
                                                       
                                                  
            else
                         bram_wr <= (others => '0');    -- read BRAM
                         addr_out(31 downto 16) <= (others => '0');   -- ctrl addr
                         addr_out(15 downto 2) <= (others => '1');   -- ctrl addr
                         addr_out(1 downto 0) <= (others => '0');   -- ctrl addr
                         ctrl_reg_write <= (others => '0');
                    
            end if;
            
            elsif(ap_done = '0' and start_prev = '1') then
                
         bram_wr <= (others => '0');    -- read BRAM
         addr_out(1 downto 0) <= "00";
         addr_out(n_addr_bits+1 downto 2) <= addr_in;
         addr_out(31 downto n_addr_bits+2) <=(others => '0'); 
         bram_en_sig <= en_in;

        end if;
        
        end process;
        
        rst_out <= not rst_in;   
        bram_en <= bram_en_sig;                                                         
                                       

end VHDL;
