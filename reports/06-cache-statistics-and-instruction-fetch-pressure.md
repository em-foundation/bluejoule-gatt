# Cache Statistics and Instruction-Fetch Pressure

## TLDR

- BlueJoule-GATT now includes Nordic cache-statistics measurements.
- The measurements cover both nRF52 and nRF54.
- The comparison is EM•Script versus Zephyr on the same board.
- The measured window is the same connection-only transaction.
- EM•Script shows less instruction-cache activity on both boards.
- EM•Script shows far fewer cache misses on both boards.
- On nRF54, Zephyr has 5.3× more cache reads.
- On nRF54, Zephyr has 27.9× more cache misses.
- On nRF52, Zephyr has 1.7× more cache reads.
- On nRF52, Zephyr has 14.7× more cache misses.
- Same-board comparisons are the meaningful comparisons.
- The data supports the tiny-code locality claim.
