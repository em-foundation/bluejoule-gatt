# BlueJoule-GATT

BlueJoule-GATT is an experimental BLE connection benchmark focused on a short, repeatable GATT transaction.

It complements the original BlueJoule advertising benchmark. Instead of measuring only advertising energy, BlueJoule-GATT measures the energy required for a BLE peripheral to connect, complete a small application-level GATT exchange, and disconnect.

This repository currently contains Zephyr-based reference applications and milestone reports used while shaping the benchmark toward a stable 1.0 form.

## Current Scope

The current benchmark candidate uses a deterministic Zephyr central and a simple custom GATT profile.

The central:

- scans for a peripheral advertising the BlueJoule-GATT service UUID
- connects
- discovers the benchmark service by UUID
- discovers the `Command` and `Status` characteristics
- writes `Command`
- reads `Status`
- disconnects cleanly

The central uses targeted discovery. It knows the benchmark UUIDs but discovers handles at runtime.

The scored connection window excludes the preceding advertising period.

## Repository Contents

```text
central/
    Zephyr BLE central benchmark driver

peripheral/
    Zephyr BLE peripheral reference implementation

reports/
    benchmark definition notes and milestone reports

assets/
    screenshots, packet traces, and measurement images
