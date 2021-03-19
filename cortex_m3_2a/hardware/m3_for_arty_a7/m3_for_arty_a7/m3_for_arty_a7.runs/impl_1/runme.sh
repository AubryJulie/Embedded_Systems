#!/bin/sh

# 
# Vivado(TM)
# runme.sh: a Vivado-generated Runs Script for UNIX
# Copyright 1986-2019 Xilinx, Inc. All Rights Reserved.
# 

echo "This script was generated under a different operating system."
echo "Please update the PATH and LD_LIBRARY_PATH variables below, before executing this script"
exit

if [ -z "$PATH" ]; then
  PATH=D:/embedded/xilinx/SDK/2019.1/bin;D:/embedded/xilinx/Vivado/2019.1/ids_lite/ISE/bin/nt64;D:/embedded/xilinx/Vivado/2019.1/ids_lite/ISE/lib/nt64:D:/embedded/xilinx/Vivado/2019.1/bin
else
  PATH=D:/embedded/xilinx/SDK/2019.1/bin;D:/embedded/xilinx/Vivado/2019.1/ids_lite/ISE/bin/nt64;D:/embedded/xilinx/Vivado/2019.1/ids_lite/ISE/lib/nt64:D:/embedded/xilinx/Vivado/2019.1/bin:$PATH
fi
export PATH

if [ -z "$LD_LIBRARY_PATH" ]; then
  LD_LIBRARY_PATH=
else
  LD_LIBRARY_PATH=:$LD_LIBRARY_PATH
fi
export LD_LIBRARY_PATH

HD_PWD='C:/embedded/new_vivado_project/cortex_m3_5/cortex_m3_2a/hardware/m3_for_arty_a7/m3_for_arty_a7/m3_for_arty_a7.runs/impl_1'
cd "$HD_PWD"

HD_LOG=runme.log
/bin/touch $HD_LOG

ISEStep="./ISEWrap.sh"
EAStep()
{
     $ISEStep $HD_LOG "$@" >> $HD_LOG 2>&1
     if [ $? -ne 0 ]
     then
         exit
     fi
}

# pre-commands:
/bin/touch .init_design.begin.rst
EAStep vivado -log m3_for_arty_a7_wrapper.vdi -applog -m64 -product Vivado -messageDb vivado.pb -mode batch -source m3_for_arty_a7_wrapper.tcl -notrace

