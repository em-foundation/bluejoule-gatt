# Zephyr Reference Implementation

**Status:** Reference implementation milestone  
**Repository:** `bluejoule-gatt`  
**Benchmark definition:** [`01-bluejoule-gatt-definition.md`](01-bluejoule-gatt-definition.md)

## TLDR

- This report documents the first Zephyr implementation of BlueJoule-GATT.
- Zephyr is used for both the benchmark central and reference peripheral.
- The central runs on a Nordic nRF52 DK.
- The peripheral runs on a Nordic nRF54L15 DK.
- The implementation conforms to the BlueJoule-GATT v0 definition.
- The peripheral build uses about **130 KB flash** and **25.6 KB RAM**.
- The measured connection charge is about **57 µC**.
- At 3.0 V, this corresponds to about **171 µJ**.
- Advertising energy is excluded from this measurement.

## 1. Purpose

This report documents a concrete Zephyr implementation of the BlueJoule-GATT benchmark.

The benchmark itself is defined separately in:

```text
reports/01-bluejoule-gatt-definition.md
```

This report records:

- where the Zephyr source lives
- how to build the central and peripheral
- what hardware was tested
- the peripheral build size
- packet-trace evidence for the transaction
- the measured connection energy

## 2. Source Layout

```text
central/
    Zephyr BLE central benchmark driver

peripheral/
    Zephyr BLE peripheral reference implementation
```

The `central/` application drives the benchmark transaction.

The `peripheral/` application provides the Zephyr reference peripheral.

## 3. Tested Hardware

```text
central:    Nordic nRF52 DK
peripheral: Nordic nRF54L15 DK
```

## 4. Build Flow

Build and flash the central:

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

## 5. Peripheral Build Size

Current Zephyr peripheral build size:

```text
FLASH: 130,040 bytes
RAM:    25,588 bytes
```

This is the current Zephyr reference baseline.

## 6. Packet Trace

The packet trace demonstrates that this Zephyr implementation completes the BlueJoule-GATT transaction defined in report 01.

Add the selected trace image here:

```markdown
![Zephyr reference packet trace](../assets/wireshark/zephyr-reference-gatt-trace.png)
```

Trace summary:

```text
connection duration: <fill in exact value>
connection events:   <fill in exact count>
```

Observed transaction:

```text
connect
targeted service discovery
targeted characteristic discovery
Command write
Status read
disconnect
```

## 7. Energy Measurement

Measured connection charge:

```text
charge:  ~57 µC
voltage: 3.0 V
energy:  ~171 µJ
```

This is the connection transaction only. Advertising before connection is excluded.

Add the selected current trace image here, if useful:

```markdown
![Zephyr reference current trace](../assets/current/zephyr-reference-current-trace.png)
```

## 8. Closing Note

This is a compliant Zephyr implementation of the current BlueJoule-GATT v0 definition.

Further Zephyr tuning may reduce image size or energy, but this report records the current reference baseline.
