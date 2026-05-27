# Coding Standards

Marut FCU mixes STM32Cube-generated project structure with handwritten embedded logic. The standards below are intended to preserve that balance.

## General Rules

- Keep changes scoped to the target and behavior you are modifying.
- Match the style already used in the file you touch.
- Prefer clarity over cleverness in control-path code.
- Treat all hardware-facing logic as safety-relevant.

## STM32Cube and Generated Files

- Do not casually rewrite generated project files.
- When editing generated C sources, prefer `USER CODE BEGIN` / `USER CODE END` regions where they exist.
- Do not move or rename `.ioc`, `.project`, `.cproject`, `.mxproject`, linker scripts, or `.settings/` content without an explicit reason.

## Embedded C Conventions

- Keep hardware constants and register-level assumptions local and explicit.
- Name task functions, sensor helpers, and MAVLink helpers descriptively.
- Use comments to explain hardware assumptions, not to restate obvious C syntax.
- Avoid introducing large abstractions unless they remove real duplication across targets.

## RTOS Conventions

- Any change to task priority, stack size, synchronization primitive, or delay timing should be called out in the PR description.
- Do not change scheduler-related behavior silently.
- If you add a task, document its purpose in the relevant target docs or architecture notes.

## Telemetry and Control Safety

- Treat message IDs, units, timing, and coordinate scaling as contracts.
- Treat RC input mapping, PWM output mapping, and PID bounds as behavior-critical interfaces.
- If you change battery scaling, sensor calibration flow, or estimator inputs, document both the code change and the expected behavioral effect.

## Cross-Target Discipline

- Do not assume one target's implementation should be copied blindly into another.
- Port fixes deliberately and identify each affected target.
- If a file exists only on one target, keep documentation honest about that.
