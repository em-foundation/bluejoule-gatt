# Peripheral firmware tools

One common Zephyr application implements ordinary BlueJoule-GATT. Platform folders identify hardware; capture identifiers additionally identify the implementation (for example `nrf-52832-zephyr`). The fixed reference central is in `../central/`.

## Choose a platform

| Platform argument | Board | Environment key | Loading tool |
| --- | --- | --- | --- |
| `nrf-52832` | Nordic nRF52 DK / nRF52832 | `nordic` | nrfutil device |
| `nrf-54l15` | Nordic nRF54L15 DK | `nordic` | nrfutil device |
| `ti-cc2340r5` | TI LP-EM-CC2340R5 | `ti` | UniFlash DSLite |
| `sil-efr32xg22e` | Silicon Labs G22E Explorer Kit | `silabs` | Simplicity Commander |
| `stm-wba55` | ST NUCLEO-WBA55CG | `st` | STM32CubeProgrammer |

Every command accepts either the short name or a folder path, including a trailing slash. For shell completion, type `platforms/` and complete the board folder. Only the last path segment selects the platform.

## Set up once

The helpers require Python 3.10 or newer and only its standard library. They do not install dependencies or require Bash. Read the relevant `platforms/<platform>/README.md` for the tested vendor environment and setup history.

Copy `environments.example.json` to `environments.local.json` and replace placeholder paths. Use absolute paths; Windows forward slashes avoid JSON escaping. This local file is excluded from Git. Paths do not need to match the maintainer's drives.

- `build.<environment>` supplies `python`, `zephyr`, and `sdk`. Python means the prepared environment's executable (`Scripts/python.exe` on Windows, usually `bin/python` on Linux/macOS). Nordic also uses `toolchain`, the bundle root. Optional `build_root` selects a shorter output location, useful for TI's long Windows filenames.
- `load.<environment>` supplies the vendor loader `executable`. TI also requires `config`, the CC2340R5 ccxml path. Select its probe through that file.

Fill in only the environments and sections you use. Loading an explicit HEX does not read build settings. No shell activation is needed: building invokes the selected environment's Python directly. The main launcher Python and the vendor Python can have different versions.

## Commands

Run from this directory:

```text
python build.py platforms/nrf-52832
python load.py platforms/nrf-52832 --probe SERIAL
python load.py platforms/nrf-52832 --probe SERIAL --hex /path/to/published.hex
python size.py platforms/nrf-52832
python size.py platforms/nrf-52832 --report all
```

| Command / option | Meaning |
| --- | --- |
| `build.py PLATFORM` | Cleanly regenerate and build the selected Zephyr target; does not flash |
| `load.py PLATFORM` | Load the existing default build image; does not build |
| `load.py --hex PATH` | Load an explicit board-compatible HEX from any implementation |
| `load.py --probe SERIAL` | Required for Nordic; optional for Silicon Labs/ST; TI uses ccxml instead |
| `load.py --recover` | Nordic only: erase user nonvolatile memory and remove access protection before loading; stops if recovery fails |
| `build.py --dry-run` / `load.py --dry-run` | Validate paths and print commands without building, recovering, or flashing |
| `size.py PLATFORM` | Read section sizes from the existing ELF without rebuilding |
| `size.py --report rom` / `ram` / `all` | Zephyr symbol/source breakdown; requires build tools and may update prerequisites |
| `--environments PATH` | All three commands: use another settings JSON instead of `environments.local.json` |
| `--help` | All three commands: show available arguments |

Never use a HEX for a different board. Platform selection currently does not validate the image's target family or memory layout. Loading programs hardware immediately unless `--dry-run` is supplied; recovery is never automatic.

## Build outputs and sizes

Default output root is `build/<platform>/`; a configured `build_root` replaces `build/` for that environment. Only the selected target is regenerated. Generated files are excluded from Git.

- Nordic uses sysbuild: the load image is `merged.hex`; application ELF and HEX are under `peripheral/zephyr/` in the platform output folder.
- Other targets use `zephyr/zephyr.hex` and `zephyr/zephyr.elf` in the platform output folder.

load.py follows the configured output root unless `--hex` overrides it. size.py selects the corresponding application ELF. Its default text/data/BSS section totals are useful for comparison; text + data approximates flash and data + BSS approximates RAM. Reserved capacity, partition padding, and runtime stack/heap high-water usage are not fully represented. Zephyr's ROM/RAM reports provide a further breakdown and can differ from the linker's memory-region summary.

## Loading notes from board testing

All five boards have been built and loaded successfully and observed working with the reference central in the maintainer's Windows setup.

- **Silicon Labs:** another attached J-Link board, including the central, can confuse probe selection. Use `--probe SERIAL` to select the intended probe, or disconnect the other board as done during testing.
- **ST WBA55:** a sleeping target may resist attachment. The working manual sequence was to hold the board reset button while starting load.py, then release it promptly after loading begins. The loader uses SWD hardware reset.
- **Nordic:** supply the correct probe serial and platform. Locked devices require `--recover`. After one recovery, J-Link failed to prepare target RAM for programming; power-cycling the target and probe resolved it. Keep verification enabled. A successful write alone does not prove that an image built for another Nordic family will run.
- **TI:** programming is followed by separate TI device-management verification. Generic readback verification disagrees with loader-managed CCFG data; the helper preserves the tested two-command recipe.

Board power, probe drivers, and physical connections remain prerequisites. Loading tools are separate installations from the Zephyr compiler environment.

## Architecture and contributions

`src/` holds the common application. `prj.conf` holds shared GATT settings. Each `platforms/<platform>/` contains board/environment selection, platform configuration, optional overlay and support source, and setup notes. CMake combines these inputs. Security-controlled permissions remain in the common source, but this release supplies ordinary GATT recipes only. Cache instrumentation is disabled by default.

Vendor improvements are welcome. Preserve the workload and fixed central; explain toolchain/configuration changes and provide build and transaction evidence. Supply captures and measurement conditions for claimed power improvements. Published results retain their original provenance when environments evolve.

No Zephyr checkout, SDK, or virtual environment is shipped here. Recorded manifests and Python inventories describe existing environments; clean setup from scratch and Linux/macOS execution have not yet been validated. Cross-benchmark sharing remains deferred while this GATT layout is proven.
