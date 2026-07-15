import io.ktor.client.HttpClient
import io.ktor.client.request.*
import io.ktor.client.statement.bodyAsChannel
import io.ktor.http.HttpStatusCode
import io.ktor.http.Url
import io.ktor.http.isSuccess
import io.ktor.utils.io.ByteReadChannel
import io.ktor.utils.io.readAvailable
import kotlinx.coroutines.*
import kotlinx.coroutines.channels.*
import org.slf4j.LoggerFactory
import java.util.concurrent.atomic.AtomicInteger
import java.io.File
import java.io.RandomAccessFile
import java.nio.ByteBuffer
import java.nio.channels.FileChannel
import java.nio.file.StandardOpenOption

data class RemoteFileInfo(
    val url: Url,
    val supportsRanges: Boolean = false,
    val length: Long? = null,
)

class Downloader(private val client: HttpClient) {
    companion object {
        private val logger = LoggerFactory.getLogger(Downloader::class.java)
    }

    suspend fun fetchFileInfo(url: Url): RemoteFileInfo {
        val response = client.head(url)
        if (response.status != HttpStatusCode.OK) throw IllegalStateException("Request failed: ${response.status}")

        val length = response.headers["Content-Length"]?.toLongOrNull()
        val supportsRanges = response.headers["Accept-Ranges"] == "bytes" && length != null

        return RemoteFileInfo(url, supportsRanges, length)
    }

    suspend fun downloadFile(fileInfo: RemoteFileInfo, output: String, connections: Int = 4, attempts: Int = 3, chunkSize: Long = 10 * 1024 * 1024) {
        if (fileInfo.supportsRanges) downloadFileChunks(fileInfo, output, connections, attempts, chunkSize)
        else downloadFileSingle(fileInfo, output)
    }

    suspend fun downloadFileSingle(fileInfo: RemoteFileInfo, output: String) = withContext(Dispatchers.IO) {
        require(!output.isBlank()) { "Output is blank" }

        val file = File(output)
        fileInfo.length?.let { length ->
            RandomAccessFile(file, "rw").use { raFile ->
                raFile.setLength(length)
            }
        }

        logger.info("Single download")

        val response = client.get(fileInfo.url)
        if (!response.status.isSuccess()) throw IllegalStateException("Request failed: ${response.status}")

        FileChannel.open(file.toPath(), StandardOpenOption.WRITE, StandardOpenOption.CREATE, StandardOpenOption.TRUNCATE_EXISTING).use { fileChannel ->
            fileChannel.write(response.bodyAsChannel())
        }
    }

    suspend fun downloadFileChunks(fileInfo: RemoteFileInfo, output: String, connections: Int = 4, attempts: Int = 3, chunkSize: Long = 10 * 1024 * 1024) = withContext(Dispatchers.IO) {
        require(fileInfo.supportsRanges) { "File doesn't support ranges" }
        require(!output.isBlank()) { "Output is blank" }
        require(connections > 0) { "Connections must be > 0" }
        require(attempts > 0) { "Attempt must be > 0" }
        require(chunkSize > 0) { "Chunk size must be > 0" }
        val length = requireNotNull(fileInfo.length)

        val file = File(output)
        RandomAccessFile(file, "rw").use { raFile ->
            raFile.setLength(length)
        }

        logger.info("Chunk download")

        FileChannel.open(file.toPath(), StandardOpenOption.WRITE, StandardOpenOption.CREATE).use { fileChannel ->
            val chunkChannel = Channel<Chunk>(Channel.UNLIMITED)
            var totalChunks = 0
            for (first in 0 until length step chunkSize) {
                val last = (first + chunkSize - 1).coerceAtMost(length - 1)
                chunkChannel.send(Chunk(first..last))
                totalChunks++
            }
            val remaining = AtomicInteger(totalChunks)

            logger.info("Total chunks: $totalChunks")

            try {
                coroutineScope {
                    repeat(connections) { index ->
                        launch {
                            for (chunk in chunkChannel) {
                                try {
                                    logger.info("Chunk ${chunk.range.first}-${chunk.range.last}: try download (connection $index)")
                                    downloadChunk(fileInfo, fileChannel, chunk.range)
                                    logger.info("Chunk ${chunk.range.first}-${chunk.range.last}: downloaded (connection $index)")
                                    if (remaining.decrementAndGet() == 0) chunkChannel.close()
                                } catch (e: CancellationException) {
                                    throw e
                                } catch (e: Exception) {
                                    logger.warn("Chunk ${chunk.range.first}-${chunk.range.last} failed, attempt ${chunk.attempts}: ${e.message}")
                                    if (chunk.attempts >= attempts) throw IllegalStateException("Chunk failed after $attempts attempts")
                                    chunkChannel.send(chunk.copy(attempts = chunk.attempts + 1))
                                }
                            }
                        }
                    }
                }
            } finally {
                chunkChannel.close()
            }
        }
    }

    private suspend fun downloadChunk(fileInfo: RemoteFileInfo, fileChannel: FileChannel, range: LongRange) {
        val response = client.get(fileInfo.url) {
            header("Range", "bytes=${range.first}-${range.last}")
        }
        if (response.status != HttpStatusCode.PartialContent) throw IllegalStateException("Request failed: ${response.status}")
        fileChannel.write(response.bodyAsChannel(), range.first)
    }

    private data class Chunk(
        val range: LongRange,
        val attempts: Int = 0
    )
}

private suspend fun FileChannel.write(readChannel: ByteReadChannel, position: Long = 0) = withContext(Dispatchers.IO) {
    val buffer = ByteBuffer.allocateDirect(8 * 1024)
    var p = position
    while (!readChannel.isClosedForRead) {
        buffer.clear()
        val read = readChannel.readAvailable(buffer)
        if (read < 0) break

        buffer.flip()

        while (buffer.hasRemaining()) {
            p += write(buffer, p)
        }
    }
}
