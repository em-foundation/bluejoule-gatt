# BlueJoule-GATT

## TLDR

* BlueJoule-GATT is an experimental BLE connection benchmark.
* It complements the original BlueJoule advertising benchmark.
* BlueJoule measured advertising energy; BlueJoule-GATT scores the connection transaction after advertising.
* Advertising is used to establish the connection, but advertising energy is excluded from the score.
* The scored transaction is: connect, discover, write `Command`, read `Status`, disconnect.
* EM•Scope provides repeatable event-based scoring and preserved measurement artifacts.
* The benchmark now runs across Nordic nRF52, Nordic nRF54, nRF54 FLPR, and TI CC2340R5 targets.
* EM•Script scores higher than Zephyr on same-device Nordic and TI comparisons.
* On nRF54, EM•Script now runs on both the Cortex-M33 application core and the RISC-V FLPR core with same-class measured energy.
* Event drilldowns and on-chip core comparisons show that tiny EM•Script code can reduce active software cost and improve portability across constrained targets.

## Benchmark Scope

BlueJoule-GATT measures the energy required for a BLE peripheral to connect, complete a small application-level GATT exchange, and disconnect.

The benchmark is intentionally the complement of the original BlueJoule advertising benchmark. Advertising is still required to establish the connection, but the scored BlueJoule-GATT window begins after advertising and excludes the preceding advertising period.

The benchmark candidate uses a deterministic BLE central and a simple custom GATT profile.

The central:

* scans for a peripheral advertising the BlueJoule-GATT service UUID
* connects
* discovers the benchmark service by UUID
* discovers the `Command` and `Status` characteristics
* writes `Command`
* reads `Status`
* disconnects cleanly

The central uses targeted discovery. It knows the benchmark UUIDs but discovers handles at runtime.

This repository tracks work toward a stable BlueJoule-GATT 1.0 benchmark definition.

## Latest Results

BlueJoule-GATT now has automated EM•Scope scores across multiple hardware and software implementations.

The table below reports the 10 s EM•erald score for each measured configuration, using the event-based BlueJoule-GATT scoring path.

| Platform     | Implementation             |      10 s Score | Notes                                              |
| ------------ | -------------------------- | --------------: | -------------------------------------------------- |
| Nordic nRF54 | EM•Script / RISC-V FLPR    | 60.83 EM•eralds | Report 9 back-to-back current-codebase run         |
| Nordic nRF54 | EM•Script / Cortex-M33     | 60.82 EM•eralds | Report 9 back-to-back current-codebase run         |
| Nordic nRF54 | Zephyr / Cortex-M33        | 46.00 EM•eralds | Existing nRF54 Zephyr reference                    |
| TI CC2340R5  | EM•Script / SRAM           | 34.37 EM•eralds | Best TI CC2340R5 score measured so far             |
| TI CC2340R5  | EM•Script / flash/cache    | 33.19 EM•eralds | Primary TI EM•Script score used in Report 7        |
| Nordic nRF52 | EM•Script                  | 30.44 EM•eralds | Higher than Nordic nRF52 Zephyr                    |
| Nordic nRF52 | Zephyr                     | 28.16 EM•eralds | Nordic nRF52 Zephyr reference                      |
| TI CC2340R5  | Zephyr                     | 19.49 EM•eralds | TI Zephyr baseline                                 |
| TI CC2340R5  | SimpleLink                 | 10.95 EM•eralds | TI mature BLE stack baseline                       |

The same-device comparisons remain the most important comparisons:

* Nordic nRF52: EM•Script scores 8% higher than Zephyr.
* Nordic nRF54 Cortex-M33: EM•Script scores 32% higher than Zephyr on the same application core.
* Nordic nRF54 FLPR: EM•Script scores in the same class as the current Cortex-M33 EM•Script build while running on the RISC-V FLPR core.
* TI CC2340R5: EM•Script flash/cache scores 70% higher than Zephyr and about 3.0× higher than SimpleLink.
* TI CC2340R5: EM•Script SRAM improves the TI EM•Script result further, but is treated as a secondary execution-locality result rather than the primary stack-comparison baseline.

The nRF54 EM•Script Cortex-M33 and FLPR rows use the current Report 9 back-to-back measurement pair. Earlier reports used an older nRF54 Cortex-M33 EM•Script score of 58.28 EM•eralds; the current table uses the newer same-codebase measurement.

The cross-device comparison is interesting but should be interpreted more carefully. The Nordic nRF54 remains the highest-scoring platform in this measurement set. The TI CC2340R5 EM•Script results are notable because they exceed the Nordic nRF52 scores, primarily due to the TI device’s very low long-idle sleep current.

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

## Reports

1. [BlueJoule-GATT Definition](reports/01-bluejoule-gatt-definition.md)  
   Defines the benchmark profile, central/peripheral requirements, discovery model, measurement window, and scoring intent.

2. [Zephyr Reference Implementation](reports/02-zephyr-reference-implementation.md)  
   Documents the current Zephyr central/peripheral reference implementation, tested hardware, build flow, packet trace, and baseline measurement.

3. [EM•Script Candidate Implementation](reports/03-emscript-candidate-implementation.md)  
   Documents the EM•Script candidate implementation and same-hardware comparison against the Zephyr reference, including an early matched event-level comparison.

4. [EM•Scope Measurement Workflow](reports/04-emscope-measurement-workflow.md)  
   Documents the automated repeated-measurement workflow and first EM•Scope scores.

5. [nRF52 Results and Cross-Generation Comparison](reports/05-nrf52-results-and-cross-generation-comparison.md)  
   Documents the nRF52 EM•Script port, the four-way nRF52/nRF54 score matrix, and the cross-generation comparison.

6. [Cache Statistics and Instruction-Fetch Pressure](reports/06-cache-statistics-and-instruction-fetch-pressure.md)  
   Documents nRF52/nRF54 cache-statistics measurements comparing EM•Script and Zephyr during the same connection transaction.

7. [TI CC2340R5 SimpleLink, Zephyr, and EM•Script Results](reports/07-ti-cc2340r5-results.md)  
   Documents the TI CC2340R5 same-device comparison across SimpleLink, Zephyr, and EM•Script.

8. [TI CC2340R5 Connection-Event Drilldown](reports/08-ti-cc2340r5-connection-event-drilldown.md)  
   Drills into a matched ATT Read Request event to show the software-controlled active window inside the TI CC2340R5 connection transaction.

9. [nRF54 On-Chip Core Results: Cortex-M33 vs RISC-V](reports/09-nrf54-on-chip-core-results-cortex-m33-vs-riscv.md)  
   Compares EM•Script BlueJoule-GATT on the nRF54 Cortex-M33 application core and RISC-V FLPR core.

## Related Projects

* [BlueJoule](https://github.com/em-foundation/BlueJoule/blob/main/docs/ReadMore.md)
* [EM•Scope](https://github.com/em-foundation/emscope/blob/docs-stable/docs/ReadMore.md)
* [EM•Script](https://www.openem.org/emscript/)

## Status

BlueJoule-GATT is not yet a finalized benchmark.

Current work is focused on:

* refining the benchmark toward a stable 1.0 definition
* validating repeated EM•Scope measurement runs
* preserving scored capture artifacts
* comparing general-purpose BLE stack behavior with the EM•Script candidate
* extending the benchmark across additional platforms and measurement conditions

Discussion and feedback are welcome as the benchmark definition evolves.

<p align="right">
  <sub>
    drafted with ChatGPT &ndash; reviewed/approved by
    <a href="https://github.com/biosbob">@biosbob</a>
  </sub>
</p>
