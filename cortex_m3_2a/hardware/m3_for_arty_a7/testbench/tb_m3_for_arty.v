//-----------------------------------------------------------------------------
// The confidential and proprietary information contained in this file may
// only be used by a person authorised under and to the extent permitted
// by a subsisting licensing agreement from ARM limited.
//
//            (C) COPYRIGHT 2006-2018 ARM limited.
//                ALL RIGHTS RESERVED
//
// This entire notice must be reproduced on all copies of this file
// and copies of this file may only be made by a person if such person is
// permitted to do so under the terms of a subsisting license agreement
// from ARM limited.
//
//      SVN Information
//
//      Checked In          : $Date$
//
//      Revision            : $Revision$
//
//      Release Information : Cortex-M3 DesignStart-r0p1-00rel0
//-----------------------------------------------------------------------------
// Purpose :
//     Testbench for the M3 on Arty A7 board example design
//-----------------------------------------------------------------------------

// Uncomment this if the model for the Micron and Spansion QSPI devices has been downloaded
// and installed in the appropriate directory.  If this is not defined, the testbench will issue
// an error message if there are any attempts to read or write to the QSPI
// `define INCLUDE_QSPI_MODEL 1




`timescale 1 ps / 1 ps

module tb_m3_for_arty ();

    // Define input clock
    localparam CLK_PERIOD_SYS  = 10000;

    reg         clk_sys  = 1'b0;
    reg         reset_n;
    reg         nTRST;

    
   

    wire  [3:0] led_4bits_tri_io;
    reg   [3:0] button_4bits_tri_i;


    
    always #(CLK_PERIOD_SYS/2)  clk_sys  <= ~clk_sys;
    localparam SW_INI_RATE = 800000;
    localparam SW_TOGGLE_RATE = 300000;
            


    
    // Release resets on appropriate clock edges
    initial
    begin
        reset_n                  = 1'b0;
        nTRST                    = 1'b0;
        button_4bits_tri_i = 4'b0;
        
        repeat (500) @(posedge clk_sys) begin end;
        reset_n = 1'b1;
        repeat (100) @(posedge clk_sys) begin end;
        nTRST   = 1'b1;
        while( led_4bits_tri_io == 4'hf )
            repeat (50) @(posedge clk_sys) begin end;
        $display("end of reset");
        repeat (SW_INI_RATE) @(posedge clk_sys) begin end;
        $display("end of ini");
        // Simulate a push of the button.
        repeat(20) begin
            repeat (SW_TOGGLE_RATE) @(posedge clk_sys) begin end;
            button_4bits_tri_i = button_4bits_tri_i ^ 1;
        end
        $display("end of sim");
    end

    // LEDs are driven by tri-state.  Add pull-ups
    pullup i_led_4bits_pu[3:0] (led_4bits_tri_io);
    
    // Instantiate appropriate DUT
`ifdef ARTY_S7_TARGET
    m3_for_arty_s7 DUT (
`else
    m3_for_arty_a7_wrapper DUT (
`endif
        // Clocks and reset
        .reset                      (reset_n),      // Negative reset.  Poorly labelled in board file
        .sys_clock                  (clk_sys),
        // Debug
        .nTRST                      (nTRST),
        .TDI                        (1'b0),
        
        // Push buttons
        .push_buttons_4bits_tri_i   (button_4bits_tri_i),

        // LEDs
        .led_4bits_tri_o           (led_4bits_tri_io)
        );

endmodule : tb_m3_for_arty
