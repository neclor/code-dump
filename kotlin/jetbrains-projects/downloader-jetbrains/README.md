# downloader-jetbrains

A parallel file downloader CLI built with Kotlin, Ktor, and Clikt.

## Features

- Parallel downloading using HTTP Range requests
- Automatic fallback to single-threaded download if the server doesn't support ranges
- Configurable number of parallel connections
- Configurable chunk size
- Configurable retry attempts per chunk
- Verbose mode with download time measurement

## Usage

```
./gradlew run --args='<URL> [options]'
```

### Options

| Option | Short | Default | Description |
|---|---|---|---|
| `--output` | `-o` | filename from URL | Output file path |
| `--connections` | `-c` | `4` | Number of parallel connections |
| `--attempts` | `-a` | `3` | Max retry attempts per chunk |
| `--chunksize` | `-s` | `10485760` (10 MB) | Size of each chunk in bytes |
| `--verbose` | `-v` | `false` | Print progress and download time |

### Examples

Download a file with default settings:
```
./gradlew run --args='https://proof.ovh.net/files/100Mb.dat'
```

Download with a custom output path, 8 parallel connections, 5 retry attempts and verbose output:
```
./gradlew run --args='https://proof.ovh.net/files/100Mb.dat -o my_file.dat -c 8 -a 5 -v'
```

Download with a custom chunk size of 5 MB:
```
./gradlew run --args='https://proof.ovh.net/files/100Mb.dat -s 5242880 -v'
```

## Testing

Unit tests use `ktor-client-mock` to simulate HTTP responses without real network calls:

Run tests:
```
./gradlew test
```

Manual testing was done using [proof.ovh.net](https://proof.ovh.net/files/) test files.
