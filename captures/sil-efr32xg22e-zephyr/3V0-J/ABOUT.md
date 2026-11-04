<!-- GENERATED FILE — DO NOT EDIT -->

<h1 align="center">Silicon Labs EFR32xG22E · Zephyr</h1>
<h3 align="center">Bench supply · 3V0</h3>


<p align="right"><sub>captured on 2026-11-01 @ 20:33:51<br>generated on 2026-11-03 @ 18:47:51</sub></p>

## Activity

- Activity: Bluetooth Low Energy peripheral connection and GATT transaction
- Role: BLE peripheral
- PHY: LE 1M
- TX power: 0 dBm
- Advertising: connectable advertising used only to establish the connection
- Advertising payload: includes the BlueJoule-GATT service UUID in the primary advertising packet
- Scored window: begins with the connection transaction and excludes preceding advertising energy
- Transaction: connect, link-layer setup, targeted service discovery, targeted characteristic discovery, write `Command`, read `Status`, disconnect, return toward idle or sleep
- Discovery: targeted to the benchmark service and characteristics; full generic GATT enumeration is not required
- Service UUID: `0000b100-0000-1000-8000-00805f9b34fb`
- Status characteristic UUID: `0000b101-0000-1000-8000-00805f9b34fb`
- Status characteristic operation: read
- Command characteristic UUID: `0000b102-0000-1000-8000-00805f9b34fb`
- Command characteristic operation: write
- Handle discovery: benchmark central knows the UUIDs but discovers handles at runtime
- Primary measured quantity: energy per completed BlueJoule-GATT transaction
- Conformance basis: observable behavior, not a canonical source implementation


## Platform

- Board model reported by Zephyr: Silicon Labs BRD2710A (xG22 Explorer Kit)
- Zephyr board target: `xg22_ek2710a`
- MCU selected by the board device tree: EFR32MG22E224F512IM40
- Flash: 512 KB
- SRAM: 32 KB
- Software stack: Silicon Labs Zephyr fork, reports Zephyr 4.4.1
- Zephyr repository: `https://github.com/siliconlabssoftware/zephyr`
- Zephyr commit: `f60fcfc44b27852017a302a08dfe236ccf94804a`
- Board target: `xg22_ek2710a`
- Toolchain: Zephyr SDK 1.0.1, GNU Arm GCC 14.3.0
- Tested host environment: Windows; Python 3.13.5, west 1.5.0
- System power management: enabled
- Controller TX-power configuration: 0 dBm
- EM2 debugging: disabled with `SL_POWER_MANAGER_INIT_EMU_EM2_DEBUG_ENABLE=0`
- Ordinary GATT; security and automatic PHY/data-length updates disabled
- Serial console and logging disabled

### References

- [Platform configuration and environment inventory](https://github.com/em-foundation/bluejoule-gatt/tree/RC-26.11/peripheral/platforms/sil-efr32xg22e)
- [Common application and build/load instructions](https://github.com/em-foundation/bluejoule-gatt/tree/RC-26.11/peripheral)

## Power Source

- Power source: regulated bench supply
- State of charge: not applicable
- Battery model: none



## EM&bull;Scope results · JS220

### 🟠&ensp;measured voltage

| &emsp;average&emsp; | &emsp;minimum&emsp; | &emsp;maximum&emsp; | &emsp;standard deviation&emsp;
|:---:|:---:|:---:|:---:|
| 2.997 V | 2.979 V | 3.004 V | 0.001 V |


### 🟠&ensp;sleep

| supply voltage | &emsp;current (avg)&emsp; | &emsp;current (std)&emsp; | &emsp;average power&emsp;
|:---:|:---:|:---:|:---:|
| 3.0 V |  0.9 µA |  3.5 µA |  2.8 µW |

### 🟠&ensp;boundary / closure

| accounting window | sleep window | event duty | closure residual | floor residual |
|:---:|:---:|:---:|:---:|:---:|
| 9.827 s | 0.100 s | 15.264% | 0.000% | -0.6 µA |

### 🟠&ensp;1&thinsp;s score

| &emsp;&emsp;event energy (avg)&emsp;&emsp; | &emsp;&emsp;event energy (std)&emsp;&emsp; | &emsp;&emsp;energy per period&emsp;&emsp; | &emsp;&emsp;energy per day&emsp;&emsp; | &emsp;&emsp;&emsp;**EM&bull;eralds**&emsp;&emsp;&emsp;
|:---:|:---:|:---:|:---:|:---:|
| 315.1 µJ |  5.4 µJ | 317.5 µJ | 27.4 J | 2.92 |

### 🟠&ensp;10&thinsp;s projection

| &emsp;&emsp;event energy (avg)&emsp;&emsp; | &emsp;&emsp;event energy (std)&emsp;&emsp; | &emsp;&emsp;energy per period&emsp;&emsp; | &emsp;&emsp;energy per day&emsp;&emsp; | &emsp;&emsp;&emsp;**EM&bull;eralds**&emsp;&emsp;&emsp;
|:---:|:---:|:---:|:---:|:---:|
| 315.1 µJ |  5.4 µJ | 343.0 µJ |  3.0 J | 26.99 |
