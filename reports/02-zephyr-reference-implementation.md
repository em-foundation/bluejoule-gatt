# Zephyr Reference Implementation

**Status:** Reference implementation milestone  
**Repository:** `bluejoule-gatt`

## TLDR

- This report describes the first BlueJoule-GATT reference implementation.
- The implementation uses Zephyr for both central and peripheral roles.
- Current tested central hardware is a Nordic nRF52 DK.
- Current tested peripheral hardware is a Nordic nRF54L15 DK.
- The central runs the fixed BlueJoule-GATT transaction.
- The peripheral exposes the benchmark service, `Status`, and `Command`.
- The central uses targeted discovery and discovers handles at runtime.
- TX power is configured at 0 dBm.
- The current connection transaction is about 115–125 ms.
- The transaction uses roughly 15–17 connection events.
- Current peripheral build size is about 130 KB flash and 25.6 KB RAM.
- Current measured connection energy is approximately 171 µJ at 3.0 V.
- Advertising energy is excluded from the connection score.
- This implementation provides the current baseline for later candidate comparisons.

## 1. Purpose

This report documents the current Zephyr-based reference implementation for BlueJoule-GATT.

The reference implementation serves three purposes:

- validate the benchmark definition
- provide a working central/peripheral implementation
- establish an initial baseline for size, packet behavior, and energy

This is the first embodiment of the benchmark, not the final benchmark specification.

## 2. Tested Hardware

Current tested configuration:

```text
central:    Nordic nRF52 DK
peripheral: Nordic nRF54L15 DK
```

The applications are expected to be portable to other Zephyr-supported BLE boards, but the above configuration is the current reference setup.

## 3. Repository Contents

```text
central/
    Zephyr BLE central benchmark driver

peripheral/
    Zephyr BLE peripheral reference implementation
```

The central and peripheral together define the current runnable reference path.

## 4. Benchmark Profile

The peripheral advertises and exposes the BlueJoule-GATT service.

```text
BlueJoule-GATT Service
    UUID: 0000b100-0000-1000-8000-00805f9b34fb
```

The service contains two characteristics:

```text
Status Characteristic
    UUID: 0000b101-0000-1000-8000-00805f9b34fb
    Properties: read

Command Characteristic
    UUID: 0000b102-0000-1000-8000-00805f9b34fb
    Properties: write
```

The central writes `Command`, then reads `Status`.

## 5. Advertising Behavior

The peripheral advertises connectably.

The primary advertising packet includes the BlueJoule-GATT service UUID.

The scan response may include a local name such as:

```text
BlueJoule-GATT
```

Advertising is used to let the central find the peripheral, but advertising energy is excluded from the scored connection transaction.

## 6. Central Transaction

The Zephyr central performs the current BlueJoule-GATT transaction:

```text
scan for advertised BlueJoule-GATT service UUID
connect
perform required BLE link-layer exchanges
discover BlueJoule-GATT service by UUID
discover Status and Command characteristics
write Command
read Status
disconnect cleanly
```

The central knows the UUIDs.

The central does not use hard-coded handles.

Handles are discovered at runtime.

## 7. Discovery Model

The central uses targeted discovery.

It discovers only:

- the BlueJoule-GATT service
- the `Status` characteristic
- the `Command` characteristic

It does not perform full generic GATT enumeration over `0x0001..0xffff`.

It does not intentionally enumerate unrelated GAP, GATT, Device Information, vendor, or stack-default services.

## 8. Connection Parameters

Current connection parameters:

```text
PHY:                 BLE 1M
connection interval: 7.5 ms
peripheral latency:  0
TX power:            0 dBm
```

The 7.5 ms connection interval drives the transaction aggressively and keeps the connection impulse short.

Current observed transaction length:

```text
duration: roughly 115–125 ms
events:   roughly 15–17 connection events
```

## 9. Peripheral Build Size

Current Zephyr peripheral build size:

```text
FLASH: 130,040 bytes
RAM:    25,588 bytes
```

This is the current trimmed Zephyr reference baseline.

Further Zephyr-specific tuning may reduce this size, but the current value is sufficient to characterize the present reference implementation.

## 10. Energy Measurement

Current measured peripheral connection charge:

```text
Zephyr reference: ~57 µC
```

At 3.0 V:

```text
~57 µC × 3.0 V ≈ 171 µJ
```

Current reference energy:

```text
approximately 171 µJ per completed BlueJoule-GATT connection transaction
```

This value excludes the advertising period before connection.

The relative comparison against other implementations is more important than the absolute number at this stage.

## 11. Packet Trace

A packet trace should be included with the reference implementation record.

Suggested asset location:

```text
assets/wireshark/zephyr-reference-gatt-trace.png
```

The expected trace shape is:

```text
CONNECT_IND
targeted service discovery
targeted characteristic discovery
Command write
Status read
LL_TERMINATE_IND
```

Empty LL data PDUs may appear between request/response pairs.

The benchmark is defined by the central transaction and measurement window, not by requiring every implementation to produce an identical packet trace.

## 12. Build Commands

From the repository root, build and flash the central:

```sh
west build -b nrf52dk/nrf52832 -d build-central central --pristine
west flash -d build-central
```

Build and flash the peripheral:

```sh
west build -b nrf54l15dk/nrf54l15/cpuapp -d build-peripheral peripheral --pristine
west flash -d build-peripheral
```

Adjust board names as needed for local hardware.

## 13. Role of This Reference

This Zephyr implementation is the current BlueJoule-GATT reference path.

It provides:

- a deterministic benchmark central
- a conventional BLE peripheral implementation
- a baseline packet trace
- a baseline image size
- a baseline energy measurement

Candidate implementations can be compared against this reference when they run the same central transaction on comparable hardware.

## 14. Current Limitations

This reference status is still preliminary.

Current scope:

- Nordic nRF52 DK central
- Nordic nRF54L15 DK peripheral
- BLE 1M PHY
- 7.5 ms connection interval
- no pairing or encryption
- no notifications
- no sustained connected idle measurement
- connection energy only, excluding advertising
- manually inspected measurement window

## 15. Next Steps

Remaining work for the reference implementation:

- add the final packet trace image under `assets/wireshark/`
- automate repeated connection cycles
- integrate with EM•Scope measurement flow
- trim captured current data to isolate connection transactions
- compute stable repeated scores
- document exact central/peripheral commits used for reported numbers
- preserve this implementation as the BlueJoule-GATT reference baseline
