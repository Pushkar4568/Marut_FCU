# Development Setup

This document covers the baseline development environment for Marut FCU firmware targets.

## Toolchain

Recommended tools from the current project documentation set:

| Tool | Recommended Version | Purpose |
|---|---|---|
| STM32CubeIDE | 1.14.x or newer in the current team workflow | Primary IDE, project import, build, debug |
| `arm-none-eabi-gcc` | bundled with CubeIDE | Cross compilation |
| STM32CubeProgrammer | 2.14.x or newer | Standalone flashing |
| ST-LINK | on-board or external | SWD flash and debug |
| Python | 3.8+ | Optional local tooling and repo automation |
| Git | 2.x | Version control |

## Supported Host Environments

The project documentation currently treats these environments as known:

- Windows 10 / 11 as the primary development host
- Ubuntu 22.04 LTS as a secondary supported host for CubeIDE-based work
- macOS as not yet validated by the current team workflow

## Clone the Repository

```bash
git clone https://github.com/TheGoodDevil00/Marut_FCU.git
cd Marut_FCU
```

## Import a Firmware Target

Each firmware target is an independent STM32CubeIDE project.

1. Open STM32CubeIDE.
2. Go to `File -> Import -> General -> Existing Projects into Workspace`.
3. Choose the specific target directory you want to work on, such as `firmware/core/`.
4. Leave `Copy projects into workspace` unchecked.
5. Finish the import.

Do not import the repository root as a single project.

## Build

Inside STM32CubeIDE:

1. Select the target project.
2. Use the `Debug` build configuration unless your team has agreed on a different one.
3. Build the project.
4. Confirm that the expected `.elf` appears in the target's `Debug/` directory.

Tracked build outputs are intentional in this repository. Do not remove them as part of ordinary cleanup.

## Flashing

### Via STM32CubeIDE

1. Connect the target board through ST-LINK over SWD.
2. Open the STM32 Cortex-M debug configuration.
3. Point the configuration at the target `.elf`.
4. Launch the debug session and resume execution after flash completes.

### Via STM32CubeProgrammer

Use STM32CubeProgrammer when IDE-based flashing is not convenient. Match the `.elf` to the exact target directory you built.

## Canonical Boards

| Target | MCU | Typical Board |
|---|---|---|
| `firmware/core` | STM32F411CEUx | custom Marut FCU board |
| `firmware/nucleo` | STM32F446RETx | NUCLEO-F446RE |
| `firmware/nf446re` | STM32F446RETx | NUCLEO-F446RE or custom F446-based bench target |
| `firmware/modes/rate` | STM32F411 | F411 breakout or equivalent bench target |
| `firmware/modes/stabilize` | STM32F411 | F411 breakout or equivalent bench target |

## Before Editing Code

Before making changes:

1. open the target `.ioc` file
2. review pin assignments and peripheral ownership
3. read the target `main.c`
4. review `CONTRIBUTING.md`, `docs/CODING_STANDARDS.md`, and `docs/TESTING.md`
