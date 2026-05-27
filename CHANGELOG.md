# Changelog

All notable changes to Marut FCU are documented here. The project follows a Keep a Changelog style, and firmware versioning follows `MAJOR.MINOR.PATCH`.

## [Unreleased]

### Added

- `firmware/nf446re`: compiled fixed-wing and VTOL task paths remain in the codebase
- repository restructuring into domain-oriented firmware, PCB, composite, and documentation areas

### In Progress

- `firmware/nf446re`: startup initialization for fixed-wing and VTOL paths remains pending hardware validation
- `pcb/marut_ground_fill`: PCB work remains in progress

## [0.1.0] - Initial Architecture Release

### Added

- unified multi-target firmware layout across `firmware/core`, `firmware/nucleo`, `firmware/nf446re`, `firmware/modes/rate`, and `firmware/modes/stabilize`
- STM32F411 integrated FCU target with RC/PPM capture, PWM actuation, attitude estimation, and MAVLink telemetry
- STM32F446 Nucleo bring-up target for bench development
- STM32F446 extended multi-mode target with quad, fixed-wing, and VTOL task paths compiled into the codebase
- isolated rate and stabilize mode targets for focused control-loop validation
- sensor-driver integrations for MPU6050 and BMP280, with QMC5883 support on applicable targets
- repository documentation scaffold and contributor workflow guidance

### Notes

- tracked `Debug/` build outputs are part of the release artifact model for this repository
