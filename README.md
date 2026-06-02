# BlueJoule-GATT

BlueJoule-GATT is an experimental BLE connection benchmark focused on a short, repeatable GATT transaction.

It complements the original BlueJoule advertising benchmark. Instead of measuring only advertising energy, BlueJoule-GATT measures the energy required for a BLE peripheral to connect, complete a small application-level GATT exchange, and disconnect.

## Latest Result

BlueJoule-GATT now has automated EM•Scope scores across two Nordic hardware generations.

Using repeated 3.0 V captures:

```text
Platform        Nordic/Zephyr    EM•Script    EM•Script gain
nRF52 @ 3.0 V      28.16          30.44          8%
nRF54 @ 3.0 V      46.00          58.28         27%
```

The EM•Script candidate scores higher than the Nordic/Zephyr reference on both nRF52 and nRF54, while using dramatically less program memory.

These results are documented in the nRF52 cross-generation report.

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
    screenshots, packet traces, measurement images, and EM•Scope report artifacts
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

4. [EM•Scope Measurement Workflow](reports/04-emscope-measurement-workflow.md)  
   Documents the automated repeated-measurement workflow and first EM•Scope scores.

5. [nRF52 Results and Cross-Generation Comparison](reports/05-nrf52-results-and-cross-generation-comparison.md)  
   Documents the nRF52 EM•Script port, the four-way nRF52/nRF54 score matrix, and the cross-generation comparison.

6. [Cache Statistics and Instruction-Fetch Pressure](reports/06-cache-statistics-and-instruction-fetch-pressure.md)  
   Documents nRF52/nRF54 cache-statistics measurements comparing EM•Script and Zephyr during the same connection transaction.

## Status

BlueJoule-GATT is not yet a finalized benchmark.

Current work is focused on:

- refining the benchmark toward a stable 1.0 definition
- validating repeated EM•Scope measurement runs
- preserving scored capture artifacts
- comparing Zephyr reference behavior with the EM•Script candidate
- extending the benchmark across additional platforms and measurement conditions

Discussion and feedback are welcome as the benchmark definition evolves.

<p align="right">
  <sub>
    drafted with ChatGPT &ndash; reviewed/approved by
    <a href="https://github.com/biosbob">@biosbob</a>
  </sub>
</p>
