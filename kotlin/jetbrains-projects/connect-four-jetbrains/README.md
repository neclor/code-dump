# Connect Four

Browser-based Connect Four built with Kotlin + Compose HTML.

[Play on GitHub Pages](https://neclor.github.io/connect-four-jetbrains/)

## Running

```bash
# Development mode with hot-reload
./gradlew jsBrowserDevelopmentRun

# Production build
./gradlew jsBrowserProductionWebpack
```

The production output is placed in `build/dist/js/productionExecutable/`.

## Tests

```bash
./gradlew jsTest
```

## Architecture

The project is split into three layers.

**Logic** (`logic/`) - pure Kotlin with no framework dependencies. Unit-tested.

**ViewModel** (`GameViewModel`) - bridges logic and UI. Holds reactive state via `mutableStateOf` and calls `GameStorage` on every change.

**UI** (`ui/`) - Composable components using Compose.

## Game State Persistence

The game state is serialized to JSON via `kotlinx.serialization` and stored in `localStorage`. The session is restored automatically on page reload.
