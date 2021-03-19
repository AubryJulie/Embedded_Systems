onbreak {quit -f}
onerror {quit -f}

vsim -voptargs="+acc" -t 1ps -L xil_defaultlib -L xpm -L lib_cdc_v1_0_2 -L proc_sys_reset_v5_0_13 -L util_vector_logic_v2_0_1 -L xlconstant_v1_1_6 -L axi_lite_ipif_v3_0_4 -L interrupt_control_v3_1_4 -L axi_gpio_v2_0_21 -L generic_baseblocks_v2_1_0 -L axi_infrastructure_v1_1_0 -L axi_register_slice_v2_1_19 -L fifo_generator_v13_2_4 -L axi_data_fifo_v2_1_18 -L axi_crossbar_v2_1_20 -L xlconcat_v2_1_3 -L axi_bram_ctrl_v4_1_1 -L blk_mem_gen_v8_4_3 -L axi_protocol_converter_v2_1_19 -L unisims_ver -L unimacro_ver -L secureip -lib xil_defaultlib xil_defaultlib.m3_for_arty_a7 xil_defaultlib.glbl

do {wave.do}

view wave
view structure
view signals

do {m3_for_arty_a7.udo}

run -all

quit -force
