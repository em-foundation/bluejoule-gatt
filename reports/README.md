# BlueJoule-GATT Development Reports

This folder preserves the original BlueJoule-GATT development reports.

These documents were produced during the design, implementation, measurement, and early analysis of the benchmark. They are retained here primarily to preserve their original repository-relative URLs and to provide a historical record of the work.

The reports are not intended to define the current repository structure or serve as the primary public presentation of BlueJoule-GATT.

## Reports

1. [BlueJoule-GATT Definition](01-bluejoule-gatt-definition.md)  
   Defines the benchmark profile, central/peripheral requirements, discovery model, measurement window, and scoring intent.

2. [Zephyr Reference Implementation](02-zephyr-reference-implementation.md)  
   Documents the Zephyr central/peripheral reference implementation, tested hardware, build flow, packet trace, and baseline measurement.

3. [EM•Script Candidate Implementation](03-emscript-candidate-implementation.md)  
   Documents the EM•Script candidate implementation and same-hardware comparison against the Zephyr reference.

4. [EM•Scope Measurement Workflow](04-emscope-measurement-workflow.md)  
   Documents the automated repeated-measurement workflow and first EM•Scope scores.

5. [nRF52 Results and Cross-Generation Comparison](05-nrf52-results-and-cross-generation-comparison.md)  
   Documents the nRF52 EM•Script port and the nRF52/nRF54 comparison.

6. [Cache Statistics and Instruction-Fetch Pressure](06-cache-statistics-and-instruction-fetch-pressure.md)  
   Documents cache-statistics measurements comparing EM•Script and Zephyr.

7. [TI CC2340R5 SimpleLink, Zephyr, and EM•Script Results](07-ti-cc2340r5-results.md)  
   Documents the TI CC2340R5 same-device comparison across SimpleLink, Zephyr, and EM•Script.

8. [TI CC2340R5 Connection-Event Drilldown](08-ti-cc2340r5-connection-event-drilldown.md)  
   Examines a matched ATT Read Request event to show software-controlled active time.

9. [nRF54 On-Chip Core Results: Cortex-M33 vs RISC-V](09-nrf54-on-chip-core-results-cortex-m33-vs-riscv.md)  
   Compares EM•Script BlueJoule-GATT on the nRF54 Cortex-M33 and FLPR RISC-V cores.

## Current Repository

The current BlueJoule-GATT benchmark backend lives in this repository, with captures and generated results organized under `captures/`.

No new development reports are currently planned for this folder. Future benchmark documentation, results, and public presentation should use the current repository structure and `bluejoule.org` as appropriate.
