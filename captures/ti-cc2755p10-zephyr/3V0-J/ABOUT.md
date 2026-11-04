<!-- GENERATED FILE — DO NOT EDIT -->

<h1 align="center">Texas Instruments CC2755P10 LaunchPad · Zephyr</h1>
<h3 align="center">Bench supply · 3V0</h3>


<p align="right"><sub>captured on 2026-11-03 @ 23:43:48<br>generated on 2026-11-04 @ 00:12:41</sub></p>

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

- Board: LP-EM-CC2755P10
- MCU: Texas Instruments CC2755P10
- Zephyr board target: `lp_em_cc2755p10`
- Software stack: TI SimpleLink Zephyr `v3.7.0-ti-9.14.02_ea`
- Toolchain: Zephyr SDK 0.16.8

### References

- [TI board documentation](https://www.ti.com/tool/LP-EM-CC2755P10)
- [BlueJoule-GATT platform configuration](https://github.com/em-foundation/bluejoule-gatt/tree/RC-26.11/peripheral/platforms/ti-cc2755p10)

## Power Source

- Power source: regulated bench supply
- State of charge: not applicable
- Battery model: none



## EM&bull;Scope results · JS220

### 🟠&ensp;measured voltage

| &emsp;average&emsp; | &emsp;minimum&emsp; | &emsp;maximum&emsp; | &emsp;standard deviation&emsp;
|:---:|:---:|:---:|:---:|
| 2.997 V | 2.985 V | 3.005 V | 0.001 V |


### 🟠&ensp;sleep

| supply voltage | &emsp;current (avg)&emsp; | &emsp;current (std)&emsp; | &emsp;average power&emsp;
|:---:|:---:|:---:|:---:|
| 3.0 V |  0.4 µA | 66.7 µA |  1.2 µW |

### 🟠&ensp;boundary / closure

| accounting window | sleep window | event duty | closure residual | floor residual |
|:---:|:---:|:---:|:---:|:---:|
| 9.847 s | 0.100 s | 15.233% | 0.000% | -0.3 µA |

### 🟠&ensp;1&thinsp;s score

| &emsp;&emsp;event energy (avg)&emsp;&emsp; | &emsp;&emsp;event energy (std)&emsp;&emsp; | &emsp;&emsp;energy per period&emsp;&emsp; | &emsp;&emsp;energy per day&emsp;&emsp; | &emsp;&emsp;&emsp;**EM&bull;eralds**&emsp;&emsp;&emsp;
|:---:|:---:|:---:|:---:|:---:|
| 546.7 µJ |  0.8 µJ | 547.6 µJ | 47.3 J | 1.69 |

### 🟠&ensp;10&thinsp;s projection

| &emsp;&emsp;event energy (avg)&emsp;&emsp; | &emsp;&emsp;event energy (std)&emsp;&emsp; | &emsp;&emsp;energy per period&emsp;&emsp; | &emsp;&emsp;energy per day&emsp;&emsp; | &emsp;&emsp;&emsp;**EM&bull;eralds**&emsp;&emsp;&emsp;
|:---:|:---:|:---:|:---:|:---:|
| 546.7 µJ |  0.8 µJ | 558.0 µJ |  4.8 J | 16.59 |

## Typical Event

<p align="center"><img src="event-B.png" alt="Event" width="900"></p>

