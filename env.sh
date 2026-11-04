#!/bin/bash
# . ./env.sh == source env.sh

export LM_LICENSE_FILE=/home/mse26/Work/TM_Rivier/Xilinx.lic

source /opt/xilinx/xrt/setup.sh
source /tools/Xilinx/Vitis/2022.2/settings64.sh

export PLATFORM_REPO_PATHS=/opt/xilinx/platforms/

# BUILD STEPS
#1. aiecompiler -> libadf.a
#2. hw -> .xo
#3. linkage -> .xsa
#4. binaire -> .xclbin
#5. host -> executable on hardware/simulateur

#cd aiengine
#make aie_compile_x86
#make aie_simulate_x86
#
#make aie_compile
#make aie_simulate
#
#cd hw
#make device TARGET=hw_emu
#
#cd bin
#export XCL_EMULATION_MODE=hw_emu
#./host sum.hw_emu.xclbin 16