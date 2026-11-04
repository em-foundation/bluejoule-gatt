# TI LP-EM-CC2755P10

Build with the existing TI Zephyr environment documented in the [CC2340R5 setup](../ti-cc2340r5/README.md). The CC2755P10 baseline uses TI SimpleLink Zephyr `v3.7.0-ti-9.14.02_ea` and Zephyr SDK 0.16.8. Use a short Windows build root.

From the peripheral directory:

```text
python build.py ti-cc2755p10
python load.py ti-cc2755p10 --dry-run
```

For loading, set `load.ti-cc2755p10.executable` to UniFlash DSLite and `load.ti-cc2755p10.config` to the CC2755P10 target configuration from your working UniFlash session. Use the same probe and power settings as that session. The loader uses the existing TI program and separate image verification commands. After inspecting the dry run, omit `--dry-run` to load. Use `--hex PATH` to select an existing compatible image.

## Ordinary GATT baseline

The 3.0 V Joulescope capture uses `emscope scan --gap 15 --min-duration 75 --sleep-window 100 --trim 10 --event-window 150`. The 10 s projection is 16.59 EM•eralds, with 546.7 µJ per event and 1.2 µW sleep power. The short post-TX plateau observed in the waveform is consistent with RX-like power. Its cause remains unverified.
