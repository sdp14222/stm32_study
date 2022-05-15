## 공부 주제
- CAN

## 캔버스 송수신 모듈
- SN65HVD230

## Target Board(STM32F407VET6)
### Clock Configuration
- HSI RC
- HCLK = 16MHz(Max : 168 MHz)
### Configuration
- Bitrate : 100kbps
- Prescaler : 2
- Time Quantum : 500.0ns
- Time Quanta in Bit Segment 1 : 14 Times
- Time Quanta in Bit Segment 2 : 5 Times
- CAN1 RX0 interrupts

### Filter
- Filter Mask ID : 0x7F3
- Filter ID : 0x106

### 보낼 메시지의 ID
- 0x102
- 0x106
- 0x10A
- 0x10E
- 수신가능 메시지의 ID는 0x102, 0x106, 0x10A, 0x10E


## Target Board(STM32F103RCT6)
### Configuration
- 100kbps
- Prescaler : 18
- Time Quantum : 500.0 ns
- Time Quanta in Bit Segment 1 : 14 Times
- Time Quanta in Bit Segment 2 : 5 Times
- USB low priority or CAN RX0 interrupts

### Filter
- Filter Mask ID : 0x7F3
- Filter ID : 0x106

### 보낼 메시지의 ID
- 보낼 메시지의 ID
- 0x102
- 0x107
- 0x10B
- 0x10E
- 수신가능 메시지의 ID는 0x102, 0x106, 0x10A, 0x10E

