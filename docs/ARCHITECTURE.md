# Architecture

Marut FCU is organized as a multi-target STM32CubeIDE repository built around a shared flight-control stack and target-specific board configurations. The active architecture spans STM32F411 and STM32F446 variants, with the long-term design goal of supporting quadcopter, fixed-wing, and VTOL operation under a unified firmware model.

## System Layers

The firmware stack used across the repository follows this structure:

| Layer | Component | Purpose |
|---|---|---|
| Application | Flight-mode tasks and control loops | Vehicle behavior and actuation |
| Middleware | FreeRTOS via CMSIS-RTOS v2 | Task scheduling, synchronization |
| Drivers | Handwritten sensor and telemetry modules | IMU, barometer, GPS, MAVLink |
| HAL | STM32 HAL | Peripheral access |
| Project generation | STM32CubeMX / STM32CubeIDE | Clock tree, pin mapping, startup scaffolding |

## Control Architecture

The core control path is centered in the `main.c` files for each target. Across the integrated targets, the firmware implements:

- arming and disarming state management
- mode switching between vehicle-control paths
- timer-capture based RC/PPM input decoding
- PWM output generation for motors or control surfaces
- sensor read and estimation pipeline
- MAVLink telemetry transmission over UART

The control loops use attitude estimation derived from the `MPU6050` and altitude estimation from the `BMP280`. On targets where the magnetometer path is present, heading support is implemented through `QMC5883` integration.

## Task Model

The repository uses CMSIS-RTOS v2 task declarations generated from CubeMX, with handwritten task bodies in `main.c`.

Common task roles across integrated targets include:

- `ArmDisarm`: manages armed/disarmed system state
- `ModeHandler`: switches between available flight-control paths
- `Debounce_Handle`: handles button or mode-input debouncing
- `QuadTask`: executes multirotor control behavior
- `TelemetryTask`: emits MAVLink telemetry

The F446 multi-mode target also defines:

- `FixedWingTask`: fixed-wing control-path logic
- `VtolMode`: VTOL mode logic

Those two paths are compiled into the codebase, but their startup initialization remains disabled pending hardware validation.

## Sensors and Interfaces

The codebase currently exposes these main functional modules:

| Module | Typical Source | Function |
|---|---|---|
| `mpu6050` | `Core/Src/mpu6050.c` | IMU readout and attitude estimation |
| `bmp280` | `Core/Src/bmp280.c` | Pressure and altitude estimation |
| `gps_parser` | target-specific `Core/Src/gps_parser.c` | NMEA parsing and fix extraction |
| `mav_messages` | `Core/Src/mav_messages.c` | MAVLink packet assembly and UART transmit |
| `qmc5883` / `qmcr5883l` | target-dependent | Magnetometer heading path |

## Target Strategy

The firmware tree is split into:

- `firmware/core`: primary custom-board integration target
- `firmware/nucleo`: F446 bring-up and bench-development target
- `firmware/nf446re`: extended multi-mode development target
- `firmware/modes/rate`: isolated rate-mode validation target
- `firmware/modes/stabilize`: isolated stabilize-mode validation target

The isolated mode targets exist to validate loop behavior with less system complexity before integrating changes into the main FCU targets.
