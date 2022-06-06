## 공부 주제
- Ethernet
- LwIP
- UDP

## 모듈
- DP83848 PHY Transceiver

## ETH

### Mode
- RMII

### Configuration
- Advanced Parameters
  - External PHY Configuration
    - PHY : DP83848_PHY_ADDRESS

## MiddleWares
### LwIP
- LwIP : Enabled

### Configuration
- General Settings
  - IPv4 - DHCP Options
    - LWIP_DHCP(DHCP Module) : Disabled

  - IP Address Settings
    - IP_ADDRESS(IP Address) : 192.168.0.27
    - NETMASK_ADDRESS(Netmask Address) : 255.255.255.0
    - GATEWAY_ADDRESS(Gateway Address) : 192.168.0.1

- Platform Settings
  - Name : Driver_PHY
  - IPs or Components : DP83848
  - Found Solutions : DP83848


## Reference
- STM32Cube_FW_F4_V1.27.0 - LwIP_UDP_Echo_Server Example


## Pin Connect
- ETH_REF_CLK - OSCIN


## Error1
- STM32CubeIDE version 1.9.0
- Hard fault 증상
  - MX_LWIP_Init() - lwip_init() - mem_init() - mem->prev = 0;

### Solve  
- STM32F4에서 0x30044000은 유효한 램 주소값이 아님
- lwipopts.h에서 추가
```
/* USER CODE BEGIN 1 */
#define CHECKSUM_GEN_ICMP 0
#define CHECKSUM_CHECK_ICMP 0
#undef LWIP_SUPPORT_CUSTOM_PBUF
#undef LWIP_RAM_HEAP_POINTER
/* USER CODE END 1 */
```
- [관련 링크](https://community.st.com/s/question/0D53W00001ScqS1SAJ/lwip-get-hardfaulthandler-after-update-on-stm32cubeide-v190)


## Error2
- 처음 전원 공급하고 ping 보내면 "요청 시간이 만료되었습니다"가 나옴
- 하지만 시간이 조금 지나면 "대상 호스트에 연결할 수 없습니다"가 나옴

