//Copyright 1986-2019 Xilinx, Inc. All Rights Reserved.
//--------------------------------------------------------------------------------
//Tool Version: Vivado v.2019.1 (win64) Build 2552052 Fri May 24 14:49:42 MDT 2019
//Date        : Wed Mar 17 11:48:32 2021
//Host        : LAPTOP-DO70CIAJ running 64-bit major release  (build 9200)
//Command     : generate_target m3_for_arty_a7_wrapper.bd
//Design      : m3_for_arty_a7_wrapper
//Purpose     : IP block netlist
//--------------------------------------------------------------------------------
`timescale 1 ps / 1 ps

module m3_for_arty_a7_wrapper
   (SWCLK,
    TDI,
    led_4bits_tri_o,
    locked,
    nTRST,
    push_buttons_4bits_tri_i,
    reset,
    sys_clock);
  input SWCLK;
  input TDI;
  output [3:0]led_4bits_tri_o;
  output locked;
  input nTRST;
  input [3:0]push_buttons_4bits_tri_i;
  input reset;
  input sys_clock;

  wire SWCLK;
  wire TDI;
  wire [3:0]led_4bits_tri_o;
  wire locked;
  wire nTRST;
  wire [3:0]push_buttons_4bits_tri_i;
  wire reset;
  wire sys_clock;

  m3_for_arty_a7 m3_for_arty_a7_i
       (.SWCLK(SWCLK),
        .TDI(TDI),
        .led_4bits_tri_o(led_4bits_tri_o),
        .locked(locked),
        .nTRST(nTRST),
        .push_buttons_4bits_tri_i(push_buttons_4bits_tri_i),
        .reset(reset),
        .sys_clock(sys_clock));
endmodule
