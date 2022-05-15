# STM32F407VET6 common configuration
공통 적용 사항


## System Debug mode
- Serial Wire
- SWCLK : PA14
- SWDIO : PA13

## Clock Configuration
### 13. CAN project
- HSI RC
- HCLK = 16MHz(Max : 168 MHz)

### else projects
- HSI RC - PLL Source Mux - PLLCLK
- HCLK = 100MHz(Max)
