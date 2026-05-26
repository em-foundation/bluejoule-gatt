# EM•Script BLE GATT-RW Benchmark Status

**Date:** 2026-05-25  
**Status:** Candidate implementation milestone  
**Benchmark:** BlueJoule BLE GATT-RW v1 candidate  
**Target:** Nordic nRF54L15-class hardware  
**Tooling path:** EM•Scope / BlueJoule integration pending

## TLDR

- We implemented a specialized BLE 4.2-style peripheral path in EM•Script for a proposed BlueJoule BLE GATT-RW connection benchmark.
- The transaction is intentionally small but real: connect, discover a custom service, write a `Command` characteristic, read a `Status` characteristic, then disconnect.
- The reported energy covers only the connection transaction, not the preceding advertising period.
- The benchmark central uses targeted discovery: UUIDs are known, handles are discovered, and unrelated GAP/GATT/default services are not explored.
- The current EM•Script implementation interoperates with a Zephyr central and completes the bounded transaction cleanly.
- On the same Nordic nRF54L15-class hardware at 3.0 V, the current EM•Script path measures about **135 µJ** versus about **171 µJ** for the optimized Zephyr reference, or roughly **20% lower energy**.
- The connection window is roughly **115–125 ms**, with about **15–17 connection events**.
- The current EM•Script code footprint is about **5 KB**, versus about **130 KB flash** for the trimmed Zephyr peripheral reference.
- The key architectural point is not just smaller code; it is that EM•Script can specialize the BLE/GATT path around a known application profile at build time.
- Meta-domain schema knowledge enables static resources, compact handle layout, generated/specialized binding paths, tightly sized buffers, and reduced persistent state.
- Small code and compact data contribute directly to the low-energy story: less active work, better locality, less retained RAM, and faster return to sleep.
- This is not a drop-in full BLE stack. It is evidence that a profile-specialized BLE peripheral can be dramatically smaller and measurably lower-energy for a bounded transaction.
- The next milestone is to move from manually inspected runs to automated EM•Scope measurement runs, similar in spirit to the existing BLE advertising benchmark.

## 1. Purpose

This status note records the current milestone for an EM•Script implementation of a proposed BlueJoule BLE connection benchmark.

The goal is to measure the energy required for a BLE peripheral to complete a bounded GATT read/write transaction under control of a deterministic benchmark central.

This is different from the existing BLE advertising benchmark. Advertising is close to a fixed radio waveform. A connection benchmark includes protocol work: connection events, discovery, read/write behavior, acknowledgements, stack scheduling, and return to sleep.

## 2. Benchmark Transaction

The proposed benchmark profile contains one custom BlueJoule service with two characteristics:

- `Command`: writable
- `Status`: readable

The central performs this sequence:

    scan for advertised benchmark service
    connect
    discover benchmark service by UUID
    discover Command and Status characteristics
    write Command
    read Status
    disconnect

The central knows the UUIDs but does not hard-code handles. Handles are discovered during the transaction.

The current working behavior writes a command value and then reads back a status value confirming that the command was processed.

## 3. Current Implementation Status

The current EM•Script peripheral supports:

- connectable advertising
- connection establishment
- core BLE link-layer control handling
- targeted ATT/GATT discovery
- writable `Command` characteristic
- readable `Status` characteristic
- clean disconnect
- bounded current waveform suitable for energy measurement

This is a candidate implementation for the benchmark, not yet a fully automated BlueJoule / EM•Scope test.

## 4. Measurement Window

The current reported number covers only the connection transaction.

It does not include the advertising period before the central connects.

Current observed connection window:

    duration: roughly 115–125 ms
    connection events: roughly 15–17

The long-term EM•Scope benchmark flow should trim the raw waveform to isolate only the connection transaction, excluding advertising energy at the beginning of each cycle.

## 5. Energy Result

Current same-hardware result at 3.0 V:

    EM•Script: ~45 µC × 3.0 V ≈ 135 µJ
    Zephyr:    ~57 µC × 3.0 V ≈ 171 µJ

Current comparison:

    delta: roughly 36 µJ
    improvement: roughly 20% lower energy

Both measurements are on the same Nordic nRF54L15-class hardware, with TX power confirmed at 0 dBm on both sides.

The relative comparison is more important than the absolute number at this stage.

## 6. Size Result

Current EM•Script build:

    text:  4,792 bytes
    const:   292 bytes
    data:     40 bytes
    bss:     212 bytes

External shorthand:

    about 5 KB of code

Current trimmed Zephyr peripheral reference:

    FLASH: 130,040 bytes
    RAM:    25,588 bytes

The exact ratio is less important than the implication: the EM•Script implementation is small enough that code locality, instruction fetch behavior, active execution overhead, and retained-state behavior become part of the energy story.

## 7. Why EM•Script Matters

The implementation was not just hand-written small C.

It used EM•Script’s translation flow and meta-domain execution model. Because the application profile is known at build time, EM•Script can move work out of runtime and into generation/configuration.

That enables:

- schema/profile reflection in the meta domain
- generated/specialized binding thunks
- compact handle layout
- precomputed constants and responses
- direct ATT/GATT dispatch paths
- statically sized buffers based on known worst-case needs
- minimal persistent state across sleep intervals

In a conventional BLE stack, much of this remains generic runtime machinery: tables, callbacks, queues, database walkers, configuration layers, and buffers sized for broad use cases.

## 8. Why Smaller Code and Data Can Reduce Energy

The energy result is not attributed to one single mechanism.

Likely contributors include:

- less instruction fetch traffic
- better cache/code locality
- fewer generic framework paths
- less callback/database dispatch overhead
- smaller persistent state
- less RAM that must be retained during sleep
- tighter packet buffer sizing
- faster return to low-power states
- less time spent with clocks and radio-support logic active

On the current Nordic target, the EM•Script implementation can retain only the minimum RAM block across radio events. Most packet buffers do not need to survive sleep. The persistent state is mainly the small set of connection/control variables.

That small persistent footprint is part of the low-energy mechanism.

## 9. Fairness Framing

The benchmark central is deterministic.

It uses the same transaction for each peripheral:

    same central script
    same benchmark service/profile
    same targeted discovery pattern
    same read/write operation
    same disconnect behavior
    same measurement window

The central knows UUIDs but discovers handles.

Extra GAP/GATT/default services may exist on conventional stacks, but the benchmark central does not enumerate them.

Differences in packet count, connection-event count, response latency, scheduler overhead, and cleanup time are measured outcomes.

## 10. Path to EM•Scope / BlueJoule Automation

The current milestone is a manually inspected, repeatable candidate transaction.

The next benchmark milestone is to make it run in an automated EM•Scope flow.

Target flow:

- peripheral wakes roughly once per second
- peripheral advertises briefly
- central connects quickly
- central runs the fixed GATT-RW transaction
- central disconnects
- peripheral returns to sleep for the remainder of the one-second window
- EM•Scope trims the captured waveform to isolate the connection transaction
- repeated connection impulses are scored automatically

This would make the connection benchmark operate similarly to the existing BLE advertising benchmark: start the peripheral, start the central, collect several seconds of data, and compute a repeatable score.

## 11. Caveats

This is an early benchmark result.

Current scope:

- Nordic nRF54L15-class target
- BLE 4.2-style 1M PHY path
- one custom GATT-RW benchmark profile
- Zephyr central benchmark setup
- connection transaction only, excluding advertising
- manually inspected measurement window

This should not be presented as a full BLE stack or a general replacement for vendor BLE stacks.

The appropriate claim is narrower:

> EM•Script demonstrates that a highly specialized, static, profile-oriented BLE peripheral can be dramatically smaller and measurably lower-energy than a general-purpose stack for this bounded transaction.

## 12. Next Milestone

The next status report should cover automated EM•Scope measurement.

Remaining work:

- automate repeated connection transactions
- pace transactions into roughly one-second windows
- trim raw current data to exclude advertising and isolate the connection impulse
- compute stable repeated scores
- preserve the same central transaction script
- document final benchmark UUIDs/profile definition
- capture final packet/current traces for the benchmark repository

At that point, this can become a proper BlueJoule BLE connection benchmark candidate rather than only an implementation milestone.