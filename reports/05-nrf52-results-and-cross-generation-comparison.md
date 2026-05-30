# nRF52 Results and Cross-Generation Comparison

## TLDR

- BlueJoule-GATT now has results across two Nordic hardware generations.
- The EM•Script peripheral has been ported from nRF54 to nRF52.
- The port was primarily in the nRF52 radio, timer, RTC, and platform layer.
- The portable controller/profile code remained largely unchanged.
- The Zephyr reference application is essentially unchanged across platforms.
- The same benchmark transaction was used on both nRF52 and nRF54.
- The same EM•Scope scoring method was used for all four measurements.
- EM•Script outperforms the Nordic/Zephyr reference on both platforms.
- nRF54 is substantially more efficient than nRF52 for both stacks.
- EM•Script shows a larger gain on nRF54 than on nRF52.
- The result strengthens BlueJoule-GATT as a cross-generation benchmark.
- The result strengthens the tiny-code claim beyond a single chip result.

At 3.0 V:

    Platform        Nordic/Zephyr    EM•Script    EM•Script gain
    nRF52 @ 3.0 V      28.16          30.44          +8.1%
    nRF54 @ 3.0 V      46.00          58.28         +26.7%

## 1. Purpose

This report extends the earlier BlueJoule-GATT nRF54 results to nRF52, an earlier Nordic hardware generation.

The goal is to document the nRF52 EM•Script port, compare nRF52 and nRF54 measurements, and summarize the resulting four-way score matrix across two platforms and two software stacks.

The main question is whether the EM•Script advantage observed on Nordic’s current nRF54 flagship generation also appears on the earlier nRF52 generation using the same benchmark transaction and EM•Scope scoring method.

## 2. Measurement Set

This report compares four BlueJoule-GATT measurements:

    Platform        Stack
    nRF52 @ 3.0 V   Nordic/Zephyr reference
    nRF52 @ 3.0 V   EM•Script candidate
    nRF54 @ 3.0 V   Nordic/Zephyr reference
    nRF54 @ 3.0 V   EM•Script candidate

All four measurements use the same benchmark transaction:

    advertise
    connect
    targeted service discovery
    targeted characteristic discovery
    write Command
    read Status
    disconnect
    return to sleep
    repeat

All four measurements use EM•Scope scoring with the same connection-only benchmark model.

The scored event excludes the preceding advertising period and focuses on the BLE connection transaction itself.

## 3. nRF52 EM•Script Port Summary

The EM•Script BlueJoule-GATT peripheral was ported from nRF54 to nRF52.

The portable BLE controller, benchmark profile, and top-level BlueJoule-GATT behavior remained largely unchanged. The port was concentrated in the nRF52-specific radio, timer, RTC, and platform layer.

The nRF52 implementation now supports the full benchmark transaction:

    connectable advertising
    scan request / scan response
    CONNECT_IND reception
    BLE data-channel operation
    LL feature exchange
    targeted service and characteristic discovery
    Command write
    Status read
    LL terminate
    repeated EM•Scope capture/scoring

The nRF52 RTC path now provides the timing interface needed by the radio driver while internally using the 32.768 kHz RTC domain. Fine active timing, including scan-response turnaround, remains handled by the active timing path.

The resulting nRF52 EM•Script image remains in the same size class as the nRF54 implementation, with the current build at `text(4944), const(292), data(68), bss(204)`.

The completed port shows that the EM•Script BlueJoule-GATT implementation is not tied to the nRF54 radio driver.

## 4. Measurement Artifacts

Each measurement is represented by an EM•Scope-generated `ABOUT.md` report and representative event image.

Primary measurement artifacts:

- [nRF52 EM•Script candidate](../assets/captures/nrf_52_dk/emscript-3V0-P/ABOUT.md)
- [nRF52 Nordic/Zephyr reference](../assets/captures/nrf_52_dk/zephyr-3V0-P/ABOUT.md)
- [nRF54 EM•Script candidate](../assets/captures/nrf_54_dk/emscript-3V0-P/ABOUT.md)
- [nRF54 Nordic/Zephyr reference](../assets/captures/nrf_54_dk/zephyr-3V0-P/ABOUT.md)

These reports provide the source data for the score matrix below, including event energy, EM•erald score, sleep-current estimate, and representative event waveform.

The nRF54 results were introduced in earlier reports. This report adds the corresponding nRF52 results and rolls all four measurements into one comparison.


