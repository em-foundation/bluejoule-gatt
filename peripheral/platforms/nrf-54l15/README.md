# Nordic Zephyr baseline

Uses nRF Connect SDK 3.0.2, which pins Nordic's Zephyr fork and modules. Both Nordic targets share one external SDK and toolchain environment. The existing power builds used Nordic toolchain bundle 66cdf9b75e. Install NCS 3.0.2 and its compatible command-line toolchain with Nordic's SDK tools; no IDE or Bash is required to run this helper. Configure build.nordic with the bundled Python, Zephyr checkout, SDK directory and toolchain bundle root. The helper adds that bundle's bin, opt/bin and opt/bin/Scripts directories to PATH. Other hosts require native tools and have not been tested.

The project retains the dev power builds' sysbuild flow. Both platforms retain the UART-disabling power overlays (the nRF54 overlay was passed through sysbuild in the dev build). Outputs include merged.hex at the platform build root; load.py selects that complete image by default. The generated application resides in the peripheral/ subdirectory.

## Loading

Install Nordic nrfutil with its device command and probe drivers. Configure load.nordic.executable to nrfutil. This does not require the SDK or compiler. Select the attached DK with --probe SERIAL. load.py programs only firmware-touched ranges, verifies and resets; it requests recovery only with explicit --recover (which erases device user nonvolatile memory). Firmware must match the board and memory layout.

Use load.py with --hex to select a published image, or omit it for the local build. Run with --dry-run to inspect without programming. The maintainer has since built and loaded this board successfully and observed it working with the reference central on Windows.

The frozen manifest records installed repositories only; unused uncloned modules are omitted. python-packages.txt records the bundled Python inventory. Exact capture attribution and recreation from a clean installation remain separate validation steps.
