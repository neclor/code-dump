# File Deduplication Service

A multi-strategy file deduplication service built with Kotlin. Identifies and groups duplicate or visually similar files within a virtual file system, scoped per user.

## Features

- **Exact deduplication** - finds bit-for-bit identical files using SHA-256 hashing with byte-level collision verification
- **Image similarity detection** - finds visually similar images using OpenCV template matching (threshold: 0.90)
- **User-scoped results** - restricts duplicate groups to files accessible by the requesting user
- **JSON API** - processes requests via a simple JSON gateway supporting both batch and streaming responses

## Requirements

- Java 25+
- Gradle (wrapper included)
- OpenCV 4.9.0 native libraries on the system path

## Build

```bash
./gradlew build
```

Produces `Deduplicator.jar` - a self-contained fat JAR.

## Dependencies

- `kotlinx-serialization` - JSON serialization
- `opencv` 4.9.0 - image processing
- `filededup-interfaces` - VirtualFileSystem and service interfaces
