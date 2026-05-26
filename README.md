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

## Related Projects

- [BlueJoule](https://github.com/em-foundation/BlueJoule/blob/main/docs/ReadMore.md)
- [EM•Scope](https://github.com/em-foundation/emscope/blob/docs-stable/docs/ReadMore.md)
- [EM•Script](https://www.openem.org/emscript/)

## Reports

1. [BlueJoule-GATT Definition](reports/01-bluejoule-gatt-definition.md)  
   Defines the benchmark profile, central/peripheral requirements, discovery model, measurement window, and scoring intent.

2. [Zephyr Reference Implementation](reports/02-zephyr-reference-implementation.md)  
   Documents the current Zephyr central/peripheral reference implementation, tested hardware, build flow, packet trace, and baseline measurement.

3. [EM•Script Candidate Implementation](reports/03-emscript-candidate-implementation.md)  
   Documents the EM•Script candidate implementation and same-hardware comparison against the Zephyr reference.

## Status

BlueJoule-GATT is not yet a finalized benchmark.

Current work is focused on:

- stabilizing the benchmark transaction
- automating repeated EM•Scope measurement runs
- documenting the Zephyr reference behavior
- documenting early EM•Script candidate results
- refining the benchmark toward a stable 1.0 definition

Discussion and feedback are welcome as the benchmark definition evolves.

<p align="right">
  <sub>
    drafted with ChatGPT &ndash; reviewed/approved by
    <a href="https://github.com/biosbob">@biosbob</a>
  </sub>
</p>
