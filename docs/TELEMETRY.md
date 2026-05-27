# Telemetry

Marut FCU uses MAVLink over UART for ground-station telemetry. The repository contains both message helper implementations and task-level telemetry loops, and those are not perfectly identical across all targets yet.

## Ground-Station Link

- Transport: UART
- Primary implementation files: `Core/Src/mav_messages.c`, `Core/Inc/mav_messages.h`
- Recommended GCS tools:
  - Mission Planner
  - QGroundControl

## Implemented Message Helpers

Across the active targets, the repository currently contains helper functions for:

| MAVLink Message | Status in Repo | Notes |
|---|---|---|
| `HEARTBEAT` | Implemented | Armed and disarmed variants are present |
| `ATTITUDE` | Implemented | Roll and pitch derived from target-specific attitude path |
| `GPS_RAW_INT` | Implemented on GPS-capable targets | Present in `nucleo` and `nf446re`; declared in `core` headers |
| `GLOBAL_POSITION_INT` | Implemented on GPS-capable targets | Present in `nucleo` and `nf446re` |
| `BATTERY_STATUS` | Implemented | Voltage currently emitted as a fixed or placeholder value in current code |
| `SCALED_PRESSURE` | Implemented on integrated targets | Used in disarmed telemetry paths on some targets |
| `VFR_HUD` | Implemented on some targets | Not universally emitted by current loops |
| `STATUSTEXT` | Implemented on integrated targets | Used for armed/disarmed status reporting |

## Current Active Telemetry Loops

Telemetry emission varies by target:

- `firmware/core` currently emits `HEARTBEAT`, `ATTITUDE`, `BATTERY_STATUS`, `SCALED_PRESSURE`, and `STATUSTEXT` in the active telemetry loop.
- `firmware/nf446re` emits `HEARTBEAT`, `ATTITUDE`, `GLOBAL_POSITION_INT`, `BATTERY_STATUS`, `SCALED_PRESSURE`, and `STATUSTEXT` in its integrated telemetry loop.
- `firmware/nucleo` includes GPS- and global-position-related helpers and should be treated as the bench-development reference for the fuller telemetry contract.

Because target behavior still varies, contributors should verify telemetry changes on the specific target they modify.

## IDs and Units

Current message helper implementations use:

- system ID: `1`
- component ID: `200`

Current unit assumptions in helper code include:

- attitude angles in radians when serialized into MAVLink
- GPS latitude and longitude scaled to `1e7`
- altitude in millimeters for `GPS_RAW_INT` and `GLOBAL_POSITION_INT`
- battery voltage in millivolts for `BATTERY_STATUS`

## Send Rate

There is no centralized telemetry scheduler document in the codebase yet. Current task-level pacing is target-specific:

- `firmware/nf446re` telemetry task currently uses `osDelay(15)` and `osDelay(15)` in sequence inside the loop, producing an effective cadence on the order of tens of milliseconds rather than a fixed single-rate publisher.
- `firmware/core` currently delays by `osDelay(1)` inside the telemetry loop, which should be treated as implementation detail rather than a stable protocol guarantee.

Any change to telemetry cadence or message selection should be documented in `CHANGELOG.md` and called out in pull requests.
