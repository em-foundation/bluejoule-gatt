# BlueJoule-GATT

**BlueJoule-GATT** is a behavioral benchmark for measuring the energy required for a Bluetooth Low Energy peripheral to complete a prescribed connection and GATT transaction.

This repository is the technical backend for the benchmark. It contains the benchmark definition, measured capture data, references to stable platform and power-source declarations, generated result summaries, and best-effort source/build provenance.

Public presentation, comparisons, and leaderboards belong at **bluejoule.org**.

## Benchmark Activity

A conforming peripheral shall:

1. advertise connectably
2. include the BlueJoule-GATT service UUID in the primary advertising packet
3. accept a connection from the BlueJoule-GATT reference central
4. complete targeted service and characteristic discovery
5. accept one write to `Command`
6. return one read from `Status`
7. disconnect cleanly
8. return toward idle or sleep

Advertising is required to establish the connection, but advertising energy before the connection is excluded from the scored window.

### GATT Profile

| Attribute | UUID | Requirement |
|---|---|---|
| BlueJoule-GATT service | `0000b100-0000-1000-8000-00805f9b34fb` | Primary service |
| Status characteristic | `0000b101-0000-1000-8000-00805f9b34fb` | Read |
| Command characteristic | `0000b102-0000-1000-8000-00805f9b34fb` | Write |

The benchmark central knows these UUIDs but discovers handles at runtime.

Discovery is targeted to the benchmark service and its characteristics. Full generic GATT enumeration is not required.

Peripheral transmissions use the LE 1M PHY at 0 dBm.

### Reference Central

BlueJoule-GATT measurements use the reference central in [`central/`](central/). The reference central is part of the benchmark fixture: its hardware, software, connection parameters, and transaction behavior are held constant so that peripheral results are comparable.

The reference central requests a 7.5 ms connection interval, zero peripheral latency, and a 4 s supervision timeout. It uses the LE 1M PHY with automatic PHY and data-length updates disabled, and performs the benchmark without SMP/security. It discovers the benchmark service and characteristics, writes `0x04` to `Command`, reads `Status` expecting `0x84`, and disconnects.

The current reference implementation runs Zephyr on a Nordic nRF52 DK (`nrf52dk/nrf52832`). See [`central/README.md`](central/README.md) for the complete fixture definition, source, firmware image, and build provenance.

The peripheral implementation is not prescribed. Published platform implementations may be consulted as concrete examples.

## Measurement Window

The scored window covers the connection transaction only:

```text
connect
link-layer setup
targeted service discovery
targeted characteristic discovery
write Command
read Status
disconnect
return toward idle/sleep
```

The advertising period preceding the connection is excluded.

The primary measured quantity is energy per completed BlueJoule-GATT transaction.

Sleep current is the average across all included non-activity samples within the accounting scope. Any minimum or quiet sleep window is diagnostic only; it is not the sleep value used for scoring.

## Measurement

Captures are recorded and analyzed with [EM•Scope](https://github.com/em-foundation/emscope).

Each capture represents:

```text
activity
    measured on a platform
    powered by a source
    at an operating point
    observed by an analyzer
```

A platform includes the complete power-relevant execution environment: MCU, executing core, board, software stack, clocking, retained memory, build configuration, and related settings.

Raw capture data is the authoritative measurement record. Generated summaries and scores may be recomputed as tooling evolves.

## Repository Structure

```text
central/
    reference central source, provenance, and release firmware
captures/
    <vendor-prefixed-platform-id>/
        .platform
        build/
        <voltage>-<analyzer>/
```

Representative platform IDs may include:

```text
nrf-52832-emscript
nrf-54l15-emscript
nrf-54l15-flpr-emscript
ti-cc2340r5-emscript
ti-cc2340r5-emscript-sram
ti-cc2340r5-simplelink
```

A platform folder may contain:

- `.platform` — exactly one URL identifying the stable PEDS platform declaration
- `build/` — best-effort source project and generated firmware artifacts
- capture folders — operating points named `<voltage>-<analyzer>`

Current analyzer suffixes are:

```text
J    Joulescope
P    PPK2
O    Otii
```

Power-source-specific captures may be nested beneath a folder containing `.power`.

A finalized capture may contain:

```text
.emscope/
capture.yaml
analysis.yaml
emscope-capture.zip
CAPTURE.md
ABOUT.md
about.json
event-*.png
```

- `emscope-capture.zip` preserves the raw measurement and is managed through Git LFS
- `CAPTURE.md` contains optional capture-specific notes
- `ABOUT.md` is a generated human-readable result
- `about.json` is the generated machine-readable equivalent
- screenshots or other visualizations are optional

Build artifacts and source projects are retained when practical, but are not required for a valid capture.

## Scores

BlueJoule-GATT reports transaction energy, transaction duration, sleep behavior, daily energy projections, and the EM•erald energy-efficiency score generated by EM•Scope.

Same-device comparisons are the strongest use of the benchmark because radio behavior, sleep current, memory hierarchy, and platform configuration differ across devices.

