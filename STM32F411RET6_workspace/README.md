# STM32F411RET6 common configuration
공통 적용 사항  
현재 보드 인터럽트 작동 안하기 때문에 개발 중단

## System Debug mode
- Serial Wire
- SWCLK : PA14
- SWDIO : PA13

## Clock Configuration
- HSI RC - PLL Source Mux - PLLCLK
- HCLK = 100MHz(Max)
