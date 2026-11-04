# BlueJoule-GATT Reference Central

This directory contains the reference central used for BlueJoule-GATT measurements.

The reference central is part of the benchmark fixture. A different central implementation or build is not guaranteed to produce equivalent timing, packet behavior, or measured energy.

## Reference Hardware

- Board: Nordic nRF52 DK
- Target: `nrf52dk/nrf52832`

## Software and Build

- nRF Connect SDK: `v3.0.2`
- Zephyr: `4.0.99`
- Zephyr SDK/toolchain: `0b393f9e1b`
- west: `1.2.0`

Recorded build command:

```text
west build --build-dir build_dk . --pristine --board nrf52dk/nrf52832 -- -DCONFIG_DEBUG_THREAD_INFO=y
```

## Connection Parameters

- connection interval: 7.5 ms
- peripheral latency: 0
- supervision timeout: 4 s
- PHY: LE 1M
- data-length update: disabled
- automatic PHY update: disabled
- SMP/security: disabled

## Benchmark Transaction

The central passively scans for the BlueJoule-GATT service, connects, performs targeted service and characteristic discovery, writes `0x04` to `Command`, reads `Status` expecting `0x84`, disconnects, and then restarts scanning.

The source project is retained here together with the known-good release firmware artifact used for the benchmark.

## Build and load helpers

From this directory, run:

```text
python build.py
python load.py --probe CENTRAL_SERIAL --dry-run
python load.py --probe CENTRAL_SERIAL
```

Both helpers use the Nordic settings in the sibling peripheral directory's
`environments.local.json`. Use `--environments PATH` to select another file.
The build outputs `build/zephyr/zephyr.hex`. Loading defaults to that central
image. Pass `--hex PATH` to load an existing central image. Select the central
board's probe serial. Explicit `--recover` erases device user nonvolatile memory
before loading. Use the matching central fixture for each benchmark.
