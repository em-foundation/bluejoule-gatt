# BlueJoule-GATT

BlueJoule-GATT is an experimental BLE connection benchmark focused on a short, repeatable GATT transaction.

It complements the original BlueJoule advertising benchmark. Instead of measuring only advertising energy, BlueJoule-GATT measures the energy required for a BLE peripheral to connect, complete a small application-level GATT exchange, and disconnect.

This repository tracks work toward a stable BlueJoule-GATT 1.0 benchmark definition.

## Current Scope

The benchmark candidate uses a deterministic BLE central and a simple custom GATT profile.

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
```

## Reports

1. [BlueJoule-GATT v0 Definition](reports/2026-05-25-bluejoule-gatt-v0-definition.md)  
   Defines the candidate benchmark transaction, profile, targeted-discovery model, and measurement window.

2. [Zephyr Reference Status](reports/2026-05-25-zephyr-reference-status.md)  
   Documents the Zephyr central/peripheral reference implementation, packet trace, build size, and baseline measurement.

3. [EM•Script Candidate Status](reports/2026-05-25-emscript-candidate-status.md)  
   Summarizes the EM•Script candidate implementation and same-hardware comparison against the Zephyr reference.

## Status

BlueJoule-GATT is not yet a finalized benchmark.

Current work is focused on:

- stabilizing the benchmark transaction
- automating repeated EM•Scope measurement runs
- documenting the Zephyr reference behavior
- documenting early EM•Script candidate results
- refining the benchmark toward a stable 1.0 definition

Discussion and feedback are welcome as the benchmark definition evolves.
