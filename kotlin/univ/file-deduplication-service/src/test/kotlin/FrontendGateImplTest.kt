import filededuplicators.ImageFileDeduplicator
import filededuplicators.KotlinFileDeduplicator
import kotlinx.serialization.json.*
import kotlin.test.*

class FrontendGateImplTest {

    private val fileSystem = TestFileSystem()
    private val gate = FrontendGateImpl(
        KotlinFileDeduplicator(fileSystem),
        ImageFileDeduplicator(fileSystem),
    )

    @Test
    fun `accept returns success for valid exact scan`() {
        val response = gate.accept("""{
            "action": "scan_duplicates",
            "scan_type": "exact",
            "path": "/",
            "user": "alice"
        }""".trimIndent()
        )
        val json = Json.parseToJsonElement(response).jsonObject
        assertEquals("success", json["status"]?.jsonPrimitive?.content)
        assertNotNull(json["groups"]?.jsonArray)
    }

    @Test
    fun `accept returns success for valid similar scan`() {
        val response = gate.accept("""{
            "action": "scan_duplicates",
            "scan_type": "similar",
            "path": "/",
            "user": "alice"
        }""".trimIndent()
        )
        val json = Json.parseToJsonElement(response).jsonObject
        assertEquals("success", json["status"]?.jsonPrimitive?.content)
        assertNotNull(json["groups"]?.jsonArray)
    }

    @Test
    fun `accept returns error for invalid json`() {
        val response = gate.accept("not json")
        val json = Json.parseToJsonElement(response).jsonObject
        assertEquals("error", json["status"]?.jsonPrimitive?.content)
    }

    @Test
    fun `accept returns error for missing action`() {
        val response = gate.accept("""{
            "scan_type": "exact",
            "path": "/",
            "user": "alice"
        }""".trimIndent()
        )
        val json = Json.parseToJsonElement(response).jsonObject
        assertEquals("error", json["status"]?.jsonPrimitive?.content)
    }

    @Test
    fun `accept returns error for unsupported action`() {
        val response = gate.accept("""{
            "action": "unknown",
            "scan_type": "exact",
            "path": "/",
            "user": "alice"
        }""".trimIndent()
        )
        val json = Json.parseToJsonElement(response).jsonObject
        assertEquals("error", json["status"]?.jsonPrimitive?.content)
    }

    @Test
    fun `accept returns error for unsupported scan_type`() {
        val response = gate.accept("""{
            "action": "scan_duplicates",
            "scan_type": "unknown",
            "path": "/",
            "user": "alice"
        }""".trimIndent()
        )
        val json = Json.parseToJsonElement(response).jsonObject
        assertEquals("error", json["status"]?.jsonPrimitive?.content)
    }

    @Test
    fun `accept returns error for missing path`() {
        val response = gate.accept("""{
            "action": "scan_duplicates",
            "scan_type": "exact",
            "user": "alice"
        }""".trimIndent()
        )
        val json = Json.parseToJsonElement(response).jsonObject
        assertEquals("error", json["status"]?.jsonPrimitive?.content)
    }

    @Test
    fun `accept returns error for missing user`() {
        val response = gate.accept("""{
            "action": "scan_duplicates",
            "scan_type": "exact",
            "path": "/"
        }""".trimIndent()
        )
        val json = Json.parseToJsonElement(response).jsonObject
        assertEquals("error", json["status"]?.jsonPrimitive?.content)
    }

    @Test
    fun `acceptStream returns one element per duplicate group`() {
        val result = gate.acceptStream("""{
            "action": "scan_duplicates",
            "scan_type": "exact",
            "path": "/",
            "user": "alice"
        }""".trimIndent()
        ).toList()
        assertEquals(1, result.size)
    }

    @Test
    fun `acceptStream returns error stream on invalid request`() {
        val result = gate.acceptStream("not json").toList()
        assertEquals(1, result.size)
        val json = Json.parseToJsonElement(result.first()).jsonObject
        assertEquals("error", json["status"]?.jsonPrimitive?.content)
    }
}
