import com.github.ajalt.clikt.core.CliktCommand
import com.github.ajalt.clikt.parameters.options.*
import com.github.ajalt.clikt.parameters.arguments.*
import com.github.ajalt.clikt.parameters.types.int
import io.ktor.client.HttpClient
import io.ktor.client.engine.cio.CIO
import io.ktor.client.plugins.HttpTimeout
import io.ktor.http.Url
import kotlinx.coroutines.runBlocking
import ch.qos.logback.classic.Level
import ch.qos.logback.classic.Logger
import com.github.ajalt.clikt.core.CliktError
import org.slf4j.LoggerFactory

class CliCommand : CliktCommand("./gradlew run --args=") {

    private val urlString by argument(help = "URL (e.g. https://proof.ovh.net/files/100Mb.dat)")

    private val outputString by option("-o", "--output", help = "Output file").default("")

    private val connections by option("-c", "--connections").int().default(4)

    private val attempts by option("-a", "--attempts").int().default(3)

    private val chunkSize by option("-s", "--chunksize").int().default(10 * 1024 * 1024)

    private val verbose by option("-v", "--verbose").flag(default = false)

    override fun run() {
        try {
            require(urlString.isNotBlank()) { "URL cannot be empty" }
            require(connections > 0) { "Connections must be > 0" }
            require(attempts > 0) { "Attempts must be > 0" }
            require(chunkSize > 0) { "Chunk size must be > 0" }

            val url = Url(urlString)
            val output: String = if (outputString.isBlank()) url.encodedPath.substringAfterLast('/') else outputString
            if (verbose) {
                val logger = LoggerFactory.getLogger(Downloader::class.java) as Logger
                logger.level = Level.INFO
            }

            if (verbose) println("Url: $url, Output: '$output', Connections: $connections, Chunk size: $chunkSize")

            runBlocking {
                HttpClient(CIO) {
                    install(HttpTimeout) {
                        connectTimeoutMillis = 10_000
                        socketTimeoutMillis = 30_000
                    }
                }.use { client ->
                    val downloader = Downloader(client)

                    val fileInfo: RemoteFileInfo = downloader.fetchFileInfo(url)
                    if (verbose) println(if (fileInfo.supportsRanges) "Supports ranges ${fileInfo.length ?: ""}" else "Not supports ranges")

                    val elapsed = kotlin.time.measureTime {
                        downloader.downloadFile(fileInfo, output, connections, attempts, chunkSize.toLong())
                    }
                    if (verbose) println("Downloaded in $elapsed")
                }
            }
        } catch (e: Exception) {
            throw CliktError("Download failed: ${e.message}", cause = e)
        }
    }
}
