# Testing

Marut FCU is safety-relevant embedded firmware. Bench validation is expected before changes affecting firmware behavior are merged.

## Minimum Expectations by Change Type

| Change Type | Minimum Expectation |
|---|---|
| Documentation-only change | No firmware test required |
| Cosmetic code change | Clean build on affected target |
| Driver or sensor change | Bench read validation on affected target |
| Control loop or PID change | Bench arm/disarm and output-response validation |
| RC/PPM capture change | Live RC channel-range validation |
| MAVLink or telemetry change | Ground-station verification |
| Timer, clock, or interrupt change | Full bench run; scope or logic-analyzer evidence recommended |
| RTOS task structure change | Multi-task stress run with overflow and fault monitoring |

## Firmware Test Checklist

Before opening a PR that changes firmware:

- confirm the affected target builds cleanly
- confirm the target `.elf` is produced in `Debug/`
- validate that no new warnings or unexpected binary growth appeared
- validate the affected runtime behavior on bench hardware where possible
- document what was tested and what was not tested

## What a PR Should Report

For firmware-impacting pull requests, include:

- affected target(s)
- board used for validation
- toolchain / IDE version if relevant
- test setup summary
- completed checklist items
- known untested paths
- any anomalies observed

## Isolated Validation Targets

`firmware/modes/rate` and `firmware/modes/stabilize` exist to support focused loop and timing validation outside the full integrated stack. Use them when isolating a control-path change before promoting it into the integrated targets.

## Current Limitation

There is no repository-wide automated CI build-and-test pipeline enforcing these checks today. Contributors are responsible for honest bench-validation reporting.
