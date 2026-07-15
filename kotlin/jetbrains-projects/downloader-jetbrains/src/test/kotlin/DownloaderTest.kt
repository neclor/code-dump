import io.ktor.client.HttpClient
import io.ktor.client.engine.mock.MockEngine
import io.ktor.client.engine.mock.respond
import io.ktor.http.*
import io.ktor.utils.io.ByteReadChannel
import kotlinx.coroutines.runBlocking
import java.io.File
import kotlin.test.*

class DownloaderTest {

    private val url = Url("http://test/file.bin")

    private fun mockClient(status: HttpStatusCode, headers: Headers = headersOf(), body: ByteArray = ByteArray(0)): HttpClient {
        return HttpClient(MockEngine) {
            engine {
                addHandler {
                    respond(
                        ByteReadChannel(body),
                        status,
                        headers
                    )
                }
            }
        }
    }

    @Test
    fun `fetchFileInfo returns correct info when server supports ranges`() = runBlocking {
        val client = mockClient(HttpStatusCode.OK, headersOf(HttpHeaders.AcceptRanges to listOf("bytes"), HttpHeaders.ContentLength to listOf("1024")))
        val info = Downloader(client).fetchFileInfo(url)
        assertEquals(1024L, info.length)
        assertTrue(info.supportsRanges)
    }

    @Test
    fun `fetchFileInfo returns supportsRanges false when no Accept-Ranges header`() = runBlocking {
        val client = mockClient(HttpStatusCode.OK, headersOf(HttpHeaders.ContentLength to listOf("1024")))
        val info = Downloader(client).fetchFileInfo(url)
        assertEquals(1024L, info.length)
        assertFalse(info.supportsRanges)
    }

    @Test
    fun `downloadFileSingle writes correct bytes to file`() = runBlocking {
        val expected = ByteArray(256) { it.toByte() }
        val client = mockClient(HttpStatusCode.OK, headersOf(HttpHeaders.ContentType, ContentType.Application.OctetStream.toString()), expected)

        val info = RemoteFileInfo(url)
        Downloader(client).downloadFileSingle(info, "test_output.bin")

        assertContentEquals(expected, File("test_output.bin").readBytes())
    }

    @Test
    fun `downloadFileChunks assembles chunks in correct order`() = runBlocking {
        val chunkSize = 64L
        val totalSize = 256L
        val expected = ByteArray(totalSize.toInt()) { it.toByte() }

        val client = HttpClient(MockEngine) {
            engine {
                addHandler { request ->
                    val range = request.headers[HttpHeaders.Range]!!.removePrefix("bytes=").split("-")
                    val from = range[0].toLong()
                    val to = range[1].toLong()
                    val body = expected.copyOfRange(from.toInt(), (to + 1).toInt())
                    respond(
                        ByteReadChannel(body),
                        HttpStatusCode.PartialContent,
                        headersOf(HttpHeaders.ContentType, ContentType.Application.OctetStream.toString())
                    )
                }
            }
        }

        val info = RemoteFileInfo(url, true, totalSize)
        Downloader(client).downloadFileChunks(info, "test_output.bin", chunkSize = chunkSize)

        assertContentEquals(expected, File("test_output.bin").readBytes())
    }

    @Test
    fun `downloadFileChunks retries failed chunks`() = runBlocking {
        val expected = ByteArray(64) { it.toByte() }
        var callCount = 0

        val client = HttpClient(MockEngine) {
            engine {
                addHandler {
                    callCount++
                    if (callCount == 1) {
                        respond(
                            ByteReadChannel(ByteArray(0)),
                            HttpStatusCode.InternalServerError
                        )
                    } else {
                        respond(
                            ByteReadChannel(expected),
                            HttpStatusCode.PartialContent,
                            headersOf(HttpHeaders.ContentType, ContentType.Application.OctetStream.toString())
                        )
                    }
                }
            }
        }

        val info = RemoteFileInfo(url, true, expected.size.toLong())
        Downloader(client).downloadFileChunks(info, "test_output.bin", chunkSize = expected.size.toLong())

        assertEquals(2, callCount)
        assertContentEquals(expected, File("test_output.bin").readBytes())
    }

    @AfterTest
    fun cleanup() {
        File("test_output.bin").delete()
    }
}