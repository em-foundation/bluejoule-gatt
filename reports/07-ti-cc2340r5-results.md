# TI CC2340R5 SimpleLink, Zephyr, and EM•Script Results

## TLDR

- BlueJoule-GATT now has three working TI CC2340R5 implementations.
- SimpleLink BLE5 is TI’s mature incumbent BLE stack.
- Zephyr is TI’s newer portable BLE path.
- EM•Script is the profile-specialized tiny-code implementation.
- All three implementations are functional and scoreable.
- All three were measured at 3.0 V using EM•Scope event-based scoring.
- The TI SimpleLink BLE5 result scores 10.95 EM•eralds.
- The TI Zephyr result scores 19.49 EM•eralds.
- The TI EM•Script result scores 33.19 EM•eralds.
- EM•Script has the lowest TI event energy measured so far.
- EM•Script has the highest TI 10 s EM•erald score measured so far.
- SimpleLink shows an unexpectedly high active floor during the connection.
- Zephyr drops much lower between connection intervals than SimpleLink.
- The TI EM•Script result extends the tiny-code comparison beyond Nordic hardware.