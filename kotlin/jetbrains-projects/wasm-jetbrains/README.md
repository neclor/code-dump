# Wasm JetBrains

Kotlin/WASM WASI project demonstrating low-level WASI API usage for reading stdin.

## Running

### Node.js
```
./gradlew wasmWasiNodeDevelopmentRun
```

### Wasmtime
```
./gradlew wasmWasiWasmtimeDevelopmentRun
```
Wasmtime is downloaded automatically on first run.

## Warning

> **Node.js on Windows**: `poll_oneoff` for stdin fails with error 57 (ENOTSOCK).  
> So if you want to test on Windows with Node.js, comment out the `waitInput(allocator)` call in `wasi/i.kt`.  
> `fd_read` blocks on input anyway, so functionality is not affected.