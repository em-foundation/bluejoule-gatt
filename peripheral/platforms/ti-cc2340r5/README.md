# TI CC2340R5

Board: LP-EM-CC2340R5, target `lp_em_cc2340r5`. This carries the existing ordinary GATT TI dev baseline, including its boot-banner default. No application source changes are needed.

## Build environment

The working Windows environment uses Python 3.10.2, CMake 3.31.10, the TI Zephyr 3.7 fork, and Zephyr SDK 0.16.8. See `west-frozen.yml` for exact repository revisions and `python-packages.txt` for the observed package inventory. These document the installed environment, not a newly validated clean installation.

Prepare the external workspace and Python 3.10 virtual environment; install its Zephyr Python requirements and the recorded tool versions, including TI's CRC tool. Configure `build.ti` in `environments.local.json`. SDK versions used for ST/Silicon Labs are not substitutes for this tested TI setup.

Run `python build.py ti-cc2340r5`. Windows TI radio-patch filenames can exceed path limits. Keep the checkout/build paths short. An optional `build_root` under `build.ti` selects a shorter absolute output directory; with that option, pass the resulting HEX explicitly to load.py. Without it, outputs use the normal `build/ti-cc2340r5` folder.

## Load environment

Install TI UniFlash with CC23xx support and XDS110 drivers. Configure `load.ti.executable` to its DSLite launcher and `load.ti.config` to the included `CC2340R5.ccxml` (or your own compatible target/probe configuration). Emporium is not required. The local example reuses the existing Emporium-installed loader only as a convenience.

Run `python load.py ti-cc2340r5 --hex /path/to/image.hex`. Omit `--hex` to select the normal local Zephyr output. Use `--dry-run` to inspect both commands without programming. Select probes through ccxml; `--probe` is not supported for TI.

The existing recipe programs with generic verification disabled, then runs TI device-management image verification separately. Generic readback verification disagrees with loader-managed CCFG data. Failure of either command stops the operation. Preserve the loader's CC23xx defaults; do not substitute mass erase/recovery operations.

Build and loading on other host systems have not been validated. Loading requires no Zephyr environment.

The workspace manifest records only the repositories actually cloned in the existing TI workspace. Unused uncloned modules are omitted. To recreate, copy west-frozen.yml into a fresh workspace's manifest/west.yml, initialize with west init -l manifest, then west update. The Zephyr checkout had no tracked local changes at snapshot time. Other module changes and a fresh installation remain unverified. When build_root is set, load.py uses that output location by default; --hex always overrides it.
