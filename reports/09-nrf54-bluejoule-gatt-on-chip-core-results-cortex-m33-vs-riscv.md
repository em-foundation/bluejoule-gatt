# nRF54 BlueJoule-GATT On-Chip Core Results: Cortex-M33 vs RISC-V

**Status:** Results milestone  
**Repository:** `bluejoule-gatt`  
**Benchmark definition:** [`01-bluejoule-gatt-definition.md`](01-bluejoule-gatt-definition.md)  
**Measurement workflow:** [`04-emscope-measurement-workflow.md`](04-emscope-measurement-workflow.md)  
**nRF54 baseline results:** [`05-nrf52-results-and-cross-generation-comparison.md`](05-nrf52-results-and-cross-generation-comparison.md)  
**Related FLPR strategy:** `L2 260619 nRF54 RISC-V EMS Strategy`  
**Tooling:** EM•Scope v25.6.x, Joulescope

## TLDR

- BlueJoule-GATT now runs on both nRF54 EM•Script targets.
- The comparison is Cortex-M33 vs RISC-V FLPR.
- Both targets share the same BlueJoule-GATT logic, BLE controller, and most nRF54 low-level drivers.
- Cortex-M33 EM•Script score: **60.82 EM•eralds**.
- RISC-V FLPR EM•Script score: **60.83 EM•eralds**.
- The scores are effectively identical.
- Both targets measured **152.2 µJ** per 10 s scoring period.
- Representative event energy is also effectively identical: **129.6 µJ** M33 vs **129.7 µJ** FLPR.
- The FLPR image is about **31% larger** than M33, better than the rough **40%** overhead seen or expected in earlier FLPR work.
- Key result: same-class energy from Arm and RISC-V, with FLPR still fitting in tiny retained SRAM.

## 1. Purpose

This report compares two EM•Script BlueJoule-GATT targets inside the Nordic nRF54L15:

- the Cortex-M33 application core
- the RISC-V FLPR core

FLPR stands for **fast lightweight peripheral processor**. In the nRF54L15, it is an on-chip RISC-V coprocessor that can access much of the same peripheral set as the main application core.

The question is narrow:

> Can the same tiny EM•Script BLE workload run on both nRF54 execution cores with comparable energy, timing, and size?

The measured answer is yes.

This is a same-device-family comparison. The benchmark profile, central behavior, measurement workflow, and EM•Scope scoring path are the same as in the existing BlueJoule-GATT reports.

## 2. Why This Comparison Matters

The nRF54L15 contains more than one useful execution target.

The existing nRF54 BlueJoule-GATT result runs on the Cortex-M33 application core. This report adds the RISC-V FLPR core as a second EM•Script execution target inside the same device family.

That makes the comparison unusually clean. It is not a comparison between different boards, different radios, or different vendors. It is a comparison between two processor targets that can drive much of the same nRF54 peripheral environment.

The FLPR core is not normally positioned as an alternative BLE application processor. It is better understood as a lightweight on-chip processor intended for lower-level peripheral-side work, front-end processing, and virtual-peripheral style use cases.

Technically, FLPR can access the broader nRF54 memory address space, including flash execute-in-place. But the more natural and interesting FLPR execution model is small SRAM-resident code. In that model, the single retained 16 KB SRAM block is a useful practical boundary for what the processor was informally meant to host.

A conventional BLE stack would normally be hard to imagine inside that kind of small SRAM-resident execution envelope. BlueJoule-GATT is different: it is a bounded BLE peripheral path rather than a full general-purpose BLE host stack.

The complete FLPR BlueJoule-GATT image fits comfortably inside one retained 16 KB SRAM block. That does not mean FLPR is architecturally limited to 16 KB. It means this BLE connection benchmark did not need more.

## 3. nRF54 Execution Targets

The Cortex-M33 application core is the main application processor and is the target used for the existing nRF54 EM•Script BlueJoule-GATT result.

The RISC-V FLPR core is implemented as a Nordic FLPR processor inside the nRF54L15. In Nordic / Zephyr target naming, these roughly correspond to:

```text
cpuapp   Cortex-M33 application core
cpuflpr  RISC-V FLPR core
```

This report uses the more descriptive names **Cortex-M33 application core** and **RISC-V FLPR core** so the comparison is understandable outside Nordic-specific tooling.

The FLPR execution model is different from the application-core model. The FLPR image is packaged with a small M33 launcher. The launcher performs the required setup, prepares the FLPR image, and starts the FLPR processor.

After launch, the BlueJoule-GATT workload runs on the FLPR core. The M33 launcher is not part of the active benchmark path; once FLPR is running, the M33 enters its low-power idle state.

For this experiment, the FLPR image runs from SRAM and is packed into one retained 16 KB SRAM block.

## 4. Shared Implementation

All EM•Script BlueJoule-GATT builds share a substantial amount of portable code. The application logic, GATT profile, portable BLE controller, connection-management logic, and benchmark transaction are intended to remain common as the workload moves from one target to another.

The nRF54 Cortex-M33 / FLPR comparison is unusual because the sharing extends much farther down into the device layer.

Both builds reuse the same nRF54 peripheral-facing code wherever practical, including:

- the nRF54 RADIO driver
- the nRF54 GPIO driver
- real-time clock and timing support
- crystal and clock-control support
- common interrupt abstractions
- most of the low-level nRF54 device support used by the benchmark

The FLPR-specific code is concentrated in the places where the execution environment is genuinely different:

- FLPR startup
- M33 launcher and handoff
- linker layout and retained-SRAM placement
- FLPR interrupt entry details
- FLPR-specific sleep management

The Cortex-M33 and FLPR builds are therefore not two unrelated BLE implementations, and they are not two unrelated nRF54 driver stacks. They are mostly the same EM•Script BLE workload and the same nRF54 device code compiled for two different execution cores.

## 5. Measurement Setup

The Cortex-M33 and FLPR measurements use the standard BlueJoule-GATT workflow.

The benchmark transaction is the same one used throughout the report series:

```text
connect
discover service
discover characteristics
write Command
read Status
disconnect
```

Advertising is required to establish the connection, but the scored BlueJoule-GATT window excludes the preceding advertising period.

The peripheral runs on the nRF54L15 DK. The central is the same deterministic BlueJoule-GATT central used for the existing benchmark runs. Both nRF54 EM•Script targets use the same 10 s event-period EM•Scope score used for the other BlueJoule-GATT targets.

The Cortex-M33 and FLPR results were taken back-to-back using the current EM•Script nRF54 codebase. That matters because the nRF54 EM•Script implementation continued to evolve during FLPR bring-up, and most of that code is shared between the two targets.

The earlier published Cortex-M33 score remains useful historical context, but this report uses the back-to-back Cortex-M33 / FLPR pair for the same-chip comparison.

## 6. Score and Energy Results

The headline result is that the current Cortex-M33 and FLPR EM•Script builds are effectively identical within measurement resolution.

```text
Platform  Implementation              10 s Score        Notes
--------  --------------------------  ---------------   -----------------------------------------
nRF54     EM•Script / Cortex-M33      60.82 EM•eralds   Current codebase; back-to-back with FLPR
nRF54     EM•Script / RISC-V FLPR     60.83 EM•eralds   Current codebase; back-to-back with Cortex-M33
nRF54     EM•Script / Cortex-M33      58.28 EM•eralds   Prior published run on earlier codebase
nRF54     Zephyr / Cortex-M33         46.00 EM•eralds   Existing nRF54 Zephyr reference
```

The most important comparison is the current-codebase EM•Script pair:

```text
Cortex-M33 application core:  60.82 EM•eralds
RISC-V FLPR core:             60.83 EM•eralds
```

For practical purposes, those results are the same.

The underlying EM•Scope energy numbers show the same pattern:

```text
Target                  Sleep Power   Event Energy   Energy / 10 s Period   10 s Score
---------------------   -----------   ------------   --------------------   ----------
Cortex-M33 EM•Script       2.3 µW       129.6 µJ          152.2 µJ           60.82 EM•eralds
RISC-V FLPR EM•Script      2.3 µW       129.7 µJ          152.2 µJ           60.83 EM•eralds
```

The prior 58.28 EM•erald Cortex-M33 result remains historical context, but it should not be mixed with the current FLPR result to infer a core-to-core difference. The current Cortex-M33 build was rerun back-to-back with the FLPR build.

The Zephyr row is included as a reference point for the existing nRF54 general-purpose stack baseline. There is no Zephyr FLPR BlueJoule-GATT result in this report.

Because the score, event energy, period energy, and sleep power are essentially identical, this report does not include a separate matched-event drilldown. Previous BlueJoule-GATT reports used event drilldowns when the traces exposed a clear software-path difference. Here, nothing material separates the two nRF54 EM•Script targets at the event-energy level.

## 7. Program Size and Retained SRAM

The energy result is effectively tied. The main architectural difference is program size.

```text
Target                  Text     Const    Data    BSS     Total
---------------------   ------   ------   -----   -----   ------
Cortex-M33 EM•Script     6212 B    284 B    68 B   216 B   6780 B
RISC-V FLPR EM•Script    8204 B    384 B    68 B   216 B   8872 B
```

The FLPR image is larger, as expected for this RISC-V target.

```text
FLPR text overhead:        about 32%
FLPR total-size overhead:  about 31%
```

That result is better than the rough 40% overhead seen or expected in some earlier FLPR sizing work. Some of that improvement reflects the FLPR distro-shaping work already done: common-code reuse, reduced target-specific overhead, and keeping FLPR-specific support concentrated in startup, launch, retained-memory layout, interrupt entry, and sleep management.

The FLPR build also fits comfortably inside the practical retained-SRAM target used for this experiment.

```text
practical retained-SRAM target:  about 15 KB
observed FLPR image footprint:   about 8.9 KB
```

That means the full EM•Script BlueJoule-GATT peripheral path fits in one retained 16 KB SRAM block with meaningful headroom.

The FLPR execution model includes a small Cortex-M33 launcher written in EM•Script. The launcher is less than 400 bytes of generated code. It packages with the FLPR payload as a single loadable image, prepares the FLPR memory image, starts the FLPR core, and then leaves the benchmark workload running on FLPR.

This is a useful contrast with the Zephyr FLPR launcher model, where the launcher image is on the order of 30 KB. The EM•Script launcher is not a second application framework; it is just a tiny handoff mechanism.

The one-block SRAM result is not because FLPR is architecturally limited to 16 KB. The FLPR core can access the broader nRF54 memory address space, including flash execute-in-place. The point is that a small SRAM-resident execution model is the natural and interesting operating point for this processor.

The result is therefore a useful tiny-code demonstration: a real BLE connection benchmark, including the benchmark profile, controller path, radio-facing support, runtime state, and low-power execution path, fits inside the kind of small retained-memory envelope normally associated with peripheral-side firmware rather than BLE stack execution.

## 8. Interpretation

For BlueJoule-GATT, the current EM•Script Cortex-M33 and FLPR builds are in the same measured energy class.

That was not the obvious outcome. The Cortex-M33 application core is the normal place to run this kind of workload. The FLPR core is a secondary RISC-V processor intended for lighter-weight peripheral-side work. Yet the same EM•Script BLE benchmark runs on both targets and produces effectively identical energy results.

The shared implementation makes the comparison more meaningful. This is not a custom FLPR-only BLE stack and not a separate FLPR-only radio path. It is mostly the same EM•Script application, BLE controller, connection logic, and nRF54 device support compiled for two instruction-set architectures.

The visible architectural difference is size. The FLPR build is about 31% larger than the Cortex-M33 build, but still only about 8.9 KB. That is small enough to keep the complete benchmark workload inside the retained-SRAM operating point used for this experiment.

The result does not imply that FLPR should replace the Cortex-M33 as the general BLE host. It does show that a bounded, profile-specialized BLE workload can run on a secondary RISC-V core when the software is small enough.

For BlueJoule-GATT, the nRF54 result now has two useful meanings. It remains a benchmark result, showing strong EM•Script energy performance on Nordic hardware. It is also a portability result, showing that the same tiny BLE path can move from the Arm application core to the RISC-V FLPR core without a meaningful energy penalty.

## 9. RISC-V and Tiny-Code Architecture

The FLPR result also points toward a broader architecture discussion.

The point is not that the nRF54 FLPR core is the ideal processor for a BLE stack. It was designed as a lightweight on-chip processor for peripheral-side work, and this report should not be read as a claim that it should replace the Cortex-M33 application core.

The more interesting point is that FLPR is a RISC-V core inside a modern BLE SoC. RISC-V makes this kind of specialization easier to imagine. A processor can be shaped around a particular memory model, power model, peripheral role, or retained-state assumption more naturally than with a fixed commercial microcontroller core.

Tiny code changes the architecture question. If the hot software path is small enough, future MCUs may not need to assume that all meaningful protocol work requires a large general-purpose core, a large cache, or a large retained memory image.

Instead, some workloads may fit a different model:

```text
small staged runtime
small retained SRAM image
explicitly managed hot code
direct peripheral access
fast wake / short active window
return to low-power state
```

The nRF54 FLPR BlueJoule-GATT result is not proof of that future architecture. It is one data point: a real BLE connection benchmark can run from a secondary on-chip RISC-V processor when the software is small enough.

## 10. Caveats

This report compares EM•Script on the nRF54 Cortex-M33 application core against EM•Script on the nRF54 RISC-V FLPR core. It does not include a Zephyr-on-FLPR result.

The Zephyr score included earlier is the existing nRF54 Cortex-M33 Zephyr reference. It provides useful context, but it is not a same-core comparison against FLPR.

The FLPR result should also not be interpreted as a general-purpose BLE stack claim. BlueJoule-GATT is a bounded benchmark transaction with a small custom profile. It exercises a real BLE connection path, but it is not a full general-purpose BLE host stack.

The result also does not address Bluetooth qualification or certification. The point of this report is measured benchmark behavior and EMS portability, not product qualification.

The retained-SRAM result should be interpreted carefully. FLPR is not architecturally limited to one 16 KB SRAM block, and it can access more of the nRF54 memory system. The important point is that this benchmark did not need more.

The score comparison should also be read within measurement resolution. The Cortex-M33 and FLPR scores are effectively identical. The result should not be reported as FLPR being meaningfully better than the Cortex-M33.

Finally, the earlier 58.28 EM•erald Cortex-M33 result remains historical context. This report uses the current back-to-back Cortex-M33 and FLPR measurements because they were taken from the same current EM•Script codebase.

## 11. Closing Note

BlueJoule-GATT now has a same-chip Arm versus RISC-V result inside the Nordic nRF54 family.

The Cortex-M33 application core remains the normal execution target for this class of workload. The FLPR core is a secondary on-chip processor aimed at smaller peripheral-side tasks. Even so, the same EM•Script BlueJoule-GATT workload runs on both targets and produces effectively identical measured energy.

That is the important result.

The FLPR build is larger, but still small enough to fit comfortably inside the one-block retained-SRAM operating point used for this experiment. The launcher is tiny, the benchmark path is shared, and most of the nRF54 low-level device code is reused.

This adds a new row to the BlueJoule-GATT matrix, but it also adds a stronger portability statement: EM•Script can carry a real BLE connection benchmark from the nRF54 Arm application core to the nRF54 RISC-V FLPR core without changing the basic benchmark workload or giving up energy performance.

For this report, the conclusion is simple:

```text
same nRF54 device family
same EM•Script BLE workload
same measured energy class
different processor architecture
tiny retained-SRAM footprint
```

<p align="right">
  <sub>
    drafted with ChatGPT &ndash; reviewed/approved by
    <a href="https://github.com/biosbob">@biosbob</a>
  </sub>
</p>
