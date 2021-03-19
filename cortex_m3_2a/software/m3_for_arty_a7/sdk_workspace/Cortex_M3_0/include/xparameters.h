#ifndef XPARAMETERS_H   /* prevent circular inclusions */
#define XPARAMETERS_H   /* by using protection macros */

/* Definition for CPU ID */
#define XPAR_CPU_ID 0U

/* Definitions for peripheral CORTEX_M3_0 */
#define XPAR_CORTEX_M3_0_CPU_CLK_FREQ_HZ 0


/******************************************************************/

/* Canonical definitions for peripheral CORTEX_M3_0 */
#define XPAR_CPU_CORTEXM3_0_CPU_CLK_FREQ_HZ 0


/******************************************************************/


/******************************************************************/

/* Platform specific definitions */
#define PLATFORM_ARM
 
/* Definitions for sleep timer configuration */
#define XSLEEP_TIMER_IS_DEFAULT_TIMER
 
 
/******************************************************************/
/* Definitions for driver BRAM */
#define XPAR_XBRAM_NUM_INSTANCES 1U

/* Definitions for peripheral AXI_BRAM_CTRL_0 */
#define XPAR_AXI_BRAM_CTRL_0_DEVICE_ID 0U
#define XPAR_AXI_BRAM_CTRL_0_DATA_WIDTH 32U
#define XPAR_AXI_BRAM_CTRL_0_ECC 0U
#define XPAR_AXI_BRAM_CTRL_0_FAULT_INJECT 0U
#define XPAR_AXI_BRAM_CTRL_0_CE_FAILING_REGISTERS 0U
#define XPAR_AXI_BRAM_CTRL_0_UE_FAILING_REGISTERS 0U
#define XPAR_AXI_BRAM_CTRL_0_ECC_STATUS_REGISTERS 0U
#define XPAR_AXI_BRAM_CTRL_0_CE_COUNTER_WIDTH 0U
#define XPAR_AXI_BRAM_CTRL_0_ECC_ONOFF_REGISTER 0U
#define XPAR_AXI_BRAM_CTRL_0_ECC_ONOFF_RESET_VALUE 0U
#define XPAR_AXI_BRAM_CTRL_0_WRITE_ACCESS 0U
#define XPAR_AXI_BRAM_CTRL_0_S_AXI_BASEADDR 0x60000000U
#define XPAR_AXI_BRAM_CTRL_0_S_AXI_HIGHADDR 0x60001FFFU
#define XPAR_AXI_BRAM_CTRL_0_S_AXI_CTRL_BASEADDR 0xFFFFFFFFU  
#define XPAR_AXI_BRAM_CTRL_0_S_AXI_CTRL_HIGHADDR 0xFFFFFFFFU  


/******************************************************************/

/* Canonical definitions for peripheral AXI_BRAM_CTRL_0 */
#define XPAR_BRAM_0_DEVICE_ID XPAR_AXI_BRAM_CTRL_0_DEVICE_ID
#define XPAR_BRAM_0_DATA_WIDTH 32U
#define XPAR_BRAM_0_ECC 0U
#define XPAR_BRAM_0_FAULT_INJECT 0U
#define XPAR_BRAM_0_CE_FAILING_REGISTERS 0U
#define XPAR_BRAM_0_UE_FAILING_REGISTERS 0U
#define XPAR_BRAM_0_ECC_STATUS_REGISTERS 0U
#define XPAR_BRAM_0_CE_COUNTER_WIDTH 0U
#define XPAR_BRAM_0_ECC_ONOFF_REGISTER 0U
#define XPAR_BRAM_0_ECC_ONOFF_RESET_VALUE 0U
#define XPAR_BRAM_0_WRITE_ACCESS 0U
#define XPAR_BRAM_0_BASEADDR 0x60000000U
#define XPAR_BRAM_0_HIGHADDR 0x60001FFFU
#define XPAR_BRAM_0_CTRL_BASEADDR 0xFFFFFFFFU  
#define XPAR_BRAM_0_CTRL_HIGHADDR 0xFFFFFFFEU  


/******************************************************************/

/* Definitions for driver GPIO */
#define XPAR_XGPIO_NUM_INSTANCES 1

/* Definitions for peripheral AXI_GPIO_0 */
#define XPAR_AXI_GPIO_0_BASEADDR 0x40110000
#define XPAR_AXI_GPIO_0_HIGHADDR 0x4011FFFF
#define XPAR_AXI_GPIO_0_DEVICE_ID 0
#define XPAR_AXI_GPIO_0_INTERRUPT_PRESENT 1
#define XPAR_AXI_GPIO_0_IS_DUAL 1


/******************************************************************/

/* Canonical definitions for peripheral AXI_GPIO_0 */
#define XPAR_GPIO_0_BASEADDR 0x40110000
#define XPAR_GPIO_0_HIGHADDR 0x4011FFFF
#define XPAR_GPIO_0_DEVICE_ID XPAR_AXI_GPIO_0_DEVICE_ID
#define XPAR_GPIO_0_INTERRUPT_PRESENT 1
#define XPAR_GPIO_0_IS_DUAL 1


/******************************************************************/

#endif  /* end of protection macro */
