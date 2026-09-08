# Code Dump

A dumping ground for unfinished projects, university assignments, prototypes, and
throwaway experiments across a lot of languages. Nothing here is production code.

This file is a hand-maintained catalog of everything in the repo. Projects that
stand on their own link to their own README; loose collections of files just get
a one-line note.

---

## android/

| Project | What it is |
|---|---|
| `habit-tracker` | Empty Jetpack Compose app skeleton for a habit tracker — theme files only, no features yet. [README](android/habit-tracker/README.md) |
| `TestEmpty` | Compose scratchpad, playing with `LazyVerticalGrid` and `mutableStateOf`. |
| `TestNoActivity` | Bare "no activity" Android project template, untouched. |

## arduino/

| Project | What it is |
|---|---|
| `thermometer` | Thermocouple thermometer sketch: MAX6675 sensor, 16×2 LCD, relay-based temperature regulator, custom logger/config headers. |

## asm/

| Project | What it is |
|---|---|
| `test` | x86-64 assembly (Intel syntax) scratch — lowercases each `argv` string via `puts`, with a Makefile. |

## bat/

Small Windows batch/VBScript files.

- `Backup.bat` — `robocopy` mirror of important files onto a removable drive.
- `Jaba.bat` — prints ASCII art.
- `TestVirus/` — harmless self-copying / self-relaunching `.bat` and `.vbs` joke scripts.

## c/

### Standalone

| Project | What it is |
|---|---|
| `max-xor` | Maximum XOR of any pair in an array using a binary trie, with timing benchmarks over increasing input sizes. |
| `other-c` | Visual Studio solution of assorted exercises: Towers of Hanoi, past exam problems, a merge/quick sort sandbox, prime checks, Advent-of-Code-style puzzles. |
| `practice-c` | Visual Studio solution of practice projects: dynamic array (with tests), two Game of Life implementations, Snake. |

### University (`c/univ/`)

| Project | What it is |
|---|---|
| `comp-prog/project-1` | "Computer Programming" course — prefix/suffix problem, with a LaTeX report (invariants, complexity, specs). |
| `comp-prog/project-2` | Same course — an ADT-based course-scheduling project, LaTeX report, milestones. |
| `prog-tech-project` | C file-deduplication library (`filededup`: `FDInit`/`FDCheck`/`FDDump`), tests, LaTeX report. |
| `proj-prog/info0030_groupe25` | **Five or More** — GTK2 grid game (menu, board, score). Has its own [README](c/univ/proj-prog/info0030_groupe25/README.md). |
| `proj-prog/project_1` | PNM (PBM/PGM/PPM) image reader/writer. |
| `proj-prog/project_2` | PNM image filters, built on a reusable `pnm` lib, with `seatest` unit tests and Doxygen. |
| `proj-prog/project_3` | Treasure-hunt game (game/ui split, image assets). |
| `sda-project-1` | Data structures & algorithms — merge/quick sort, binary search, priority-queue autocomplete, benchmark harness, full LaTeX report. |
| `sda-project-2` | DSA — BSTs, 2D k-d trees, range search over Porto taxi-trip data, LaTeX report. |

## csharp/

| Project | What it is |
|---|---|
| `web-strategy-game` ("Sillyzation") | Blazor WebAssembly + SignalR web strategy game, split into `Client` / `Server` / `Core` / `Shared`, with a SignalR client source generator. Early WIP. [README](csharp/web-strategy-game/README.md) · [TODO](csharp/web-strategy-game/TODO.md) |
| `albion-black-market-bot` | WinForms bot for Albion Online's black market: Tesseract screen recognition, decision tree, input emulation. [README](csharp/albion-black-market-bot/README.md) |
| `Servers/WowServer` | ASP.NET WoW private-server manager — REST controllers, tmux-driven process control, JSON config. |
| `AutoClicker` | Minimal autoclicker with a low-level input handler. |
| `OldGames` | Old console games: text dungeon crawler, Tic-Tac-Toe. |
| `Sandbox` | Grab-bag of tiny projects: collection/bag test, library manager, an LC-3 VM, a math library (matrices, polynomials, linear solver, graph), dice-roll stats, storage manager. |
| `ConsoleWinformsApp` | Demo of running a console thread alongside a WinForms form. |

## flutter/

| Project | What it is |
|---|---|
| `untitled1` | Default Flutter counter starter, unmodified. |

## fpga/

| Project | What it is |
|---|---|
| `sea-battle` | VHDL Battleship for an Intel/Altera MAX-series CPLD: LED-matrix controller, RNG, single-player logic, testbench. Resource-usage notes in the [README](fpga/sea-battle/README.md). |

## godot/

| Project | What it is |
|---|---|
| `different-gravity` | Godot 4 experiment — 2D "atoms" with per-region gravity. |
| `unfinished-projects/balist-godot` | Ballistics prototype. |
| `unfinished-projects/candy-raiser-godot` | Prototype. [README](godot/unfinished-projects/candy-raiser-godot/README.md) |
| `unfinished-projects/card-game-godot` | Custom rank-based card game — full rules written up in the [README](godot/unfinished-projects/card-game-godot/README.md). |
| `unfinished-projects/expedition-survivors-godot` | Survivors-genre prototype. |
| `unfinished-projects/form-survivors-godot` | Vampire-Survivors-like, made with Godot. [README](godot/unfinished-projects/form-survivors-godot/README.md) |
| `unfinished-projects/random-survivors-godot` | "Dungeon Survivors" prototype. |
| `unfinished-projects/torpeda-godot` | Submarine / torpedo prototype (sea, ship, torpedo, tracer scenes). |

## java/

| Project | What it is |
|---|---|
| `univ/oop-project` | Java OOP course — brute-force puzzle solver (board, pieces, parser, solution model). |

## kotlin/

| Project | What it is |
|---|---|
| `univ/file-deduplication-service` | Multi-strategy file deduplication service — SHA-256 exact match plus OpenCV image-similarity detection, user-scoped, JSON API. Report included. [README](kotlin/univ/file-deduplication-service/README.md) |
| `jetbrains-projects/connect-four-jetbrains` | Browser Connect Four (Kotlin + Compose HTML), state persisted to `localStorage`. [Play on GitHub Pages](https://neclor.github.io/connect-four-jetbrains/) · [README](kotlin/jetbrains-projects/connect-four-jetbrains/README.md) |
| `jetbrains-projects/downloader-jetbrains` | Parallel file-downloader CLI (Ktor + Clikt) using HTTP range requests, with retry/fallback. [README](kotlin/jetbrains-projects/downloader-jetbrains/README.md) |
| `jetbrains-projects/wasm-jetbrains` | Kotlin/WASM WASI demo — low-level WASI calls to read stdin. [README](kotlin/jetbrains-projects/wasm-jetbrains/README.md) |
| `spring-boot-test` | Spring Boot + Kotlin CRUD demo — `Person` controller/service/repository, Thymeleaf template. |
| `bitcoin-generator` | Ktor server scaffold (from the Ktor generator) with a stub `BitcoinManager` mnemonic generator. [README](kotlin/bitcoin-generator/README.md) |
| `terraria-server-manager` | Small wrapper that starts/stops/saves a Terraria dedicated server via `ProcessBuilder`. [README](kotlin/terraria-server-manager/README.md) |

## make_block/

| Project | What it is |
|---|---|
| `Ultrasonic` | C# serial-port logger for a MakeBlock ultrasonic sensor — timestamps every time something comes within 150 units. |

## micro_bit/

BBC micro:bit scripts: `Car.py` (dodging game), `Dice.py`, `Snake.py`, `Stopwatch.py`, plus compiled `Beacon.hex` / `Radio.hex`.

## python/

| Project | What it is |
|---|---|
| `3d-engine-pygame` | Small software 3D engine on pygame — scene nodes, camera, model loading, AABB, renderer. [README](python/3d-engine-pygame/README.md) |
| `univ/thgraphesprojet` ("Graphoni") | Graph-theory project — minimum spanning tree (reverse-delete), alternate MST, dynamic MST updates as edges/nodes change, with report. [README](python/univ/thgraphesprojet/README.md) |
| `univ/wendys` | Dockerized MySQL + phpMyAdmin + Flask teaching stack, plus a layered app (repositories / models / services / dashboards) over a music-CD / event domain. [README](python/univ/wendys/README.md) |
| `univ/logic-project` | Logic course project — knight's-tour cycle and a row/column solver (2025 statement PDF included). |
| `univ/lmp` | "LMP" course Python labs — two series, labs 1–6: small programs and plotting. |
| `calculator/game` | Removed from the working tree — was a math / vectors library scratch. |
| `other/` | Standalone scripts: 8-queens, cave generator, ray casting, Project Euler 1–4 & 67, RNG, counter. |

## r/

| Project | What it is |
|---|---|
| `testproj` | R exam scripts and yogurt-nutrition plots (glucose, fat, protein analysis). |

## rust/

| Project | What it is |
|---|---|
| `little-computer-3` | LC-3 virtual machine in Rust. [README](rust/little-computer-3/README.md) |

## screeps_code/

JavaScript AI for the game **Screeps** — spawn logic, creep roles (harvester, upgrader, builder), helper modules.

## web/

| Project | What it is |
|---|---|
| `neclor.ru` | Personal site — landing page, "Netris" (Godot Tetris) web export, high-score tables, and a `test/` folder of small JS practice apps (background changer, countdown, random cat, movie modal, pagination, password generator, GitHub profiles, notes app). |
| `wow.neclor.com` | Static front-end for the WoW server manager — status panel and start/stop buttons. |
