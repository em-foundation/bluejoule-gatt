# ST NUCLEO-WBA55CG

External environment observed locally: Python 3.13.5, west 1.5.0, Zephyr 4.4.1, Zephyr SDK 1.0.1. Create a virtual environment with that Python and install the selected Zephyr checkout's `scripts/requirements.txt`, west, CMake, and Ninja. Fetch the checkout's required modules/blobs using west. Exact workspace manifests and package inventories are stored beside this file when available; local source modifications must also be preserved before claiming capture reproduction.

Use the external environment through `../../build.py`; configure paths in `../../environments.local.json`. This is a migration of existing dev build inputs, not a claim that all dependencies have been independently recreated from scratch.

ST uses CMake 4.4.4 locally. This candidate carries the latest connected-standby experiment: 500 us wakeup allowance, RAM-retaining standby overlay, nominal 0 dBm controller request, and fixed 30 ms advertising through linker wrappers. It depends on private controller symbols and may require adjustment with a different Zephyr revision. Broad powered-up intervals remain unexplained. This is a best-effort vendor improvement starting point; association with an exact historical capture is not yet verified. The UART diagnostic/PM trace variants are excluded.

## Recreate the external workspace

Create a new workspace directory outside the benchmark checkout. Copy this folder's `west-frozen.yml` to `<workspace>/manifest/west.yml`. Create a Python 3.13.5 virtual environment there, then use its Python to install `west==1.5.0`. From the workspace directory run `west init -l manifest`, followed by `west update`. Run these with the virtual environment's tools on PATH, or invoke its Python with `-m west`.

Install Zephyr SDK 1.0.1 for your host, with the ARM toolchain. Install Python dependencies with the environment's Python: `python -m pip install -r zephyr/scripts/requirements.txt`. The Silicon Labs workspace also used `bootloader/mcuboot/zephyr/requirements.txt`. `python-packages.txt` records the observed Windows package inventory, including CMake and Ninja; it is provenance, not a universal cross-host lockfile. Fetch required binary blobs using `west blobs fetch` and follow the selected SDK's license/install instructions.

Point the benchmark's local environment settings at this Python, Zephyr checkout, and SDK. The frozen manifest pins repository commits; its `self.path` is descriptive of the original manifest repository, so the copied local manifest may reside in your `manifest/` directory. Recreating these environments from zero has not yet been tested. No local tracked Zephyr changes were reported when this snapshot was recorded; changes in other modules must also be checked before attributing a historical capture.

Loading note: a sleeping board may require holding reset while starting load.py, then releasing reset promptly after the command starts. This sequence worked during maintainer testing.
