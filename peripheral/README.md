# Zephyr GATT peripheral

One common application implements the BlueJoule-GATT exchange. Platform folders identify hardware; capture identifiers additionally include the implementation suffix, such as `-zephyr`. The reference central remains in `../central/`.

## Build

The launcher requires Python 3.10 or newer, with no additional Python packages. Firmware builds use a separately prepared vendor environment. It does not install or activate environments, flash hardware, or require Bash.

1. Prepare the platform workspace, Python virtual environment, and SDK described in its README.
2. Copy `environments.example.json` to `environments.local.json`. Replace each path with your environment's absolute path. On Windows use forward slashes in JSON paths. The Python path is the environment's `Scripts/python.exe` on Windows or `bin/python` on Linux/macOS.
3. Run from this folder:

```
python BuildZephyr.py sil-efr32xg22e
python BuildZephyr.py stm-wba55
```

Use `--dry-run` to inspect the selected command. Outputs are under `build/<platform-id>/zephyr/`. Existing output for the selected target is regenerated on each build. Other targets are unaffected.

The helper invokes the selected Python's `-m west build`, puts that environment's tools on PATH, and selects its Zephyr and SDK. CMake combines the shared `prj.conf` with platform `prj.conf`, optional overlay, and `platform.cmake`. Users may invoke the printed west command directly with the same environment variables.

## Layout and contributions

`src/main.c` preserves the dev application's workload, including its configuration-controlled encrypted permissions. Cache instrumentation is retained and disabled by default. This release provides ordinary GATT builds only.

`platforms/<platform-id>/` owns board selection, platform configuration, overlays, support source, and setup notes. Common benchmark behavior belongs at the top level. Add another target by following this pattern and adding its environment entry locally.

Vendor improvements are welcome. Explain the change, preserve the GATT workload and fixed central, identify tool/source revisions and patches, and supply build and transaction evidence. Power improvements need captures and measurement conditions. A newer SDK is a new baseline requiring validation; it does not retroactively replace old capture provenance.

The current environment recipes describe tested Windows installations. Linux/macOS require native tools and recreated virtual environments; those hosts have not been validated here. Local absolute paths and generated files are ignored. No Zephyr checkout, SDK, or virtual environment is distributed here. Cross-benchmark sharing is deferred until this layout is proven.

## Load an existing HEX

Loading is independent of building. Python 3.10+ and the board's native loading tool and probe drivers are required; Zephyr, west, and a compiler are not required. Install Silicon Labs Simplicity Commander for G22E or STM32CubeProgrammer for WBA55. Configure their executable paths in the `load` section of `environments.local.json`. The `build` section is needed only by BuildZephyr. You may omit unused sections and environment entries.

```
python LoadHex.py sil-efr32xg22e --hex /path/to/published.hex
python LoadHex.py stm-wba55 --hex /path/to/published.hex
```

Without `--hex`, the image defaults to `build/<platform-id>/zephyr/zephyr.hex` under this folder. No build occurs. An explicit image may come from another implementation, provided its board and memory layout match. Platform selection does not validate image compatibility.

Use `--probe SERIAL` to select a probe (recommended when several are connected); otherwise the vendor tool selects its default. `--dry-run` validates paths and prints the command without running the loader. Without this flag, LoadHex programs the connected board and starts the image using the existing dev loading recipes. ST uses SWD hardware reset; G22E specifies its exact device. Board setup, probe firmware, access permissions, and power connections remain prerequisites.

Your local paths belong in `environments.local.json`, which is ignored by Git. The template separates `build` and `load` settings by vendor. No shell activation is necessary. Loading with an explicit HEX never reads the build section.

## TI CC2340R5

TI is also supported: `python BuildZephyr.py ti-cc2340r5` and `python LoadHex.py ti-cc2340r5`. See its platform README for the older build dependencies and UniFlash setup. Loading performs programming followed by separate TI image verification.

## Nordic

Both Nordic targets share build.nordic and load.nordic settings. Use `python BuildZephyr.py nrf-52832` or `python BuildZephyr.py nrf-54l15`. Loading requires a probe serial: `python LoadHex.py nrf-52832 --probe SERIAL` (optionally add `--hex path/to/image.hex`). See each platform README for the NCS environment and migrated single-application build.
