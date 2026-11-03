<!-- GENERATED FILE — DO NOT EDIT -->

<h1 align="center">STMicroelectronics STM32WBA55CG · Zephyr</h1>
<h3 align="center">3V0-J-v2</h3>


<p align="right"><sub>captured on 2026-11-02 @ 16:53:56<br>generated on 2026-11-03 @ 18:47:53</sub></p>

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

- Board model reported by Zephyr: STMicroelectronics STM32WBA55CG-NUCLEO board
- Zephyr board target: `nucleo_wba55cg`
- MCU selected by the board device tree: STM32WBA55CG
- Executing core: Arm Cortex-M33
- Flash: 1 MB
- SRAM reported to the application build: 128 KB
- Software stack: upstream Zephyr 4.4.1
- Zephyr repository: `https://github.com/zephyrproject-rtos/zephyr`
- Zephyr commit: `1f6485eca25431b5ff27ce9a754218c9e559bbbb`
- Board target: `nucleo_wba55cg`
- Toolchain: Zephyr SDK 1.0.1, GNU Arm GCC 14.3.0
- Tested host environment: Windows; Python 3.13.5, west 1.5.0, CMake 4.4.4
- System power management: enabled, with application-local standby integration
- TX power: application requests nominal 0 dBm through the ST controller command
- Ordinary GATT; security and automatic PHY/data-length updates disabled
- Serial console and logging disabled
- Fixed 30 ms advertising interval through the platform support wrapper
- Connected standby enabled; wakeup allowance 500 microseconds
- Device-tree overlay permits RAM-retaining standby

### References

- [Platform configuration, support source and environment inventory](https://github.com/em-foundation/bluejoule-gatt/tree/RC-26.11/peripheral/platforms/stm-wba55)
- [Common application and build/load instructions](https://github.com/em-foundation/bluejoule-gatt/tree/RC-26.11/peripheral)

## Power Source

- Power source: regulated bench supply
- State of charge: not applicable
- Battery model: none



## EM&bull;Scope results · JS220

### 🟠&ensp;measured voltage

| &emsp;average&emsp; | &emsp;minimum&emsp; | &emsp;maximum&emsp; | &emsp;standard deviation&emsp;
|:---:|:---:|:---:|:---:|
| 2.998 V | 2.977 V | 3.004 V | 0.001 V |


### 🟠&ensp;sleep

| supply voltage | &emsp;current (avg)&emsp; | &emsp;current (std)&emsp; | &emsp;average power&emsp;
|:---:|:---:|:---:|:---:|
| 3.0 V |  1.8 µA |  4.5 µA |  5.5 µW |

### 🟠&ensp;boundary / closure

| accounting window | sleep window | event duty | closure residual | floor residual |
|:---:|:---:|:---:|:---:|:---:|
| 9.370 s | 0.100 s | 16.009% | 0.000% | -0.8 µA |

### 🟠&ensp;1&thinsp;s score

| &emsp;&emsp;event energy (avg)&emsp;&emsp; | &emsp;&emsp;event energy (std)&emsp;&emsp; | &emsp;&emsp;energy per period&emsp;&emsp; | &emsp;&emsp;energy per day&emsp;&emsp; | &emsp;&emsp;&emsp;**EM&bull;eralds**&emsp;&emsp;&emsp;
|:---:|:---:|:---:|:---:|:---:|
| 594.0 µJ | 33.0 µJ | 598.7 µJ | 51.7 J | 1.55 |

### 🟠&ensp;10&thinsp;s projection

| &emsp;&emsp;event energy (avg)&emsp;&emsp; | &emsp;&emsp;event energy (std)&emsp;&emsp; | &emsp;&emsp;energy per period&emsp;&emsp; | &emsp;&emsp;energy per day&emsp;&emsp; | &emsp;&emsp;&emsp;**EM&bull;eralds**&emsp;&emsp;&emsp;
|:---:|:---:|:---:|:---:|:---:|
| 594.0 µJ | 33.0 µJ | 648.1 µJ |  5.6 J | 14.29 |
