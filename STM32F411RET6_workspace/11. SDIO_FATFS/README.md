## 공부 주제
- SDIO
- FATFS

## SDIO Setting
- Mode : SD 4 bits Wide bus
- 40MHz(48MHz Max)
- DMA : SDIO_TX, SDIO_RX
- NVIC : SDIO global interrupt enable

## Middleware
- FATFS / SD Card

## FATFS Setting
### Set Defines
- USE_LFN(Use Long Filename) : Enabled with static working buffer on the BSS

### Advanced Settings
- Use dma template : Enabled

## Example
- SDIO settings
- Using [Micro SD Card Storage Board](https://www.waveshare.com/wiki/Micro_SD_Storage_Board)
- FATFS : File Create & Write & Read
