import be.uliege.info0027.deduplication.*
import kotlinx.serialization.json.*
import java.util.stream.Stream
import filededuplicators.*

/**
 * JSON-based entry point for the frontend.
 *
 * Routes `scan_type: "exact"` to [exactFileDeduplicator] and `scan_type: "similar"` to
 * [similarFileDeduplicator]. Returns `{"status":"error",...}` on any malformed or unsupported request.
 */
class FrontendGateImpl(
    private val exactFileDeduplicator: FileDeduplicator,
    private val similarFileDeduplicator: FileDeduplicator,
) : FrontendGate {

    override fun accept(jsonRequest: String): String {
        return try {
            val request = parseRequest(jsonRequest)
            val groups = request.fileDeduplicator.findDuplicateGroups(request.user, request.path)
            buildSuccessResponse(groups)
        } catch (e: RequestException) {
            buildErrorResponse(e.message)
        }
    }

    override fun acceptStream(jsonRequest: String): Stream<String> {
        return try {
            val request = parseRequest(jsonRequest)
            val groups = request.fileDeduplicator.findDuplicateGroups(request.user, request.path)
            groups.stream().map { group ->
                Json.encodeToString(group.map { fileInfo ->
                    fileInfo.virtualPath
                })
            }
        } catch (e: RequestException) {
            Stream.of(buildErrorResponse(e.message))
        }
    }

    private fun parseRequest(jsonRequest: String): Request {
        val jsonObject = try {
            Json.parseToJsonElement(jsonRequest).jsonObject
        } catch (_: Exception) {
            throw RequestException("Invalid JSON request")
        }

        val action = jsonObject["action"]?.jsonPrimitive?.contentOrNull ?: throw RequestException("Missing action")
        if (action != "scan_duplicates") throw RequestException("Unsupported action: $action")

        val scanType = jsonObject["scan_type"]?.jsonPrimitive?.contentOrNull ?: throw RequestException("Missing scan_type")
        val fileDeduplicator = when (scanType) {
            "exact" -> exactFileDeduplicator
            "similar" -> similarFileDeduplicator
            else -> throw RequestException("Unsupported scan_type: $scanType")
        }

        val path = jsonObject["path"]?.jsonPrimitive?.contentOrNull ?: throw RequestException("Missing path")

        val user = jsonObject["user"]?.jsonPrimitive?.contentOrNull ?: throw RequestException("Missing user")

        return Request(fileDeduplicator, path, user)
    }

    private fun buildSuccessResponse(groups: Collection<Set<VirtualFileInfo>>): String =
        buildJsonObject {
            put("status", "success")
            put("groups", buildJsonArray {
                groups.forEach { group ->
                    add(buildJsonArray {
                        group.forEach { fileInfo ->
                            add(fileInfo.virtualPath)
                        }
                    })
                }
            })
        }.toString()

    private fun buildErrorResponse(message: String?): String =
        buildJsonObject {
            put("status", "error")
            put("message", message ?: "Unknown error")
        }.toString()

    private class RequestException(message: String) : Exception(message)
    private data class Request(
        val fileDeduplicator: FileDeduplicator,
        val path: String,
        val user: String
    )
}
