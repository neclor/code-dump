import filededuplicators.KotlinFileDeduplicator
import java.nio.file.Files
import java.nio.file.Path
import kotlin.test.*

class KotlinFileDeduplicatorTest {

    private val fileSystem = TestFileSystem()
    private val deduplicator = KotlinFileDeduplicator(fileSystem)

    @Test
    fun `alice has one duplicate group across root`() {
        val groups = deduplicator.findDuplicateGroups("alice", "/")
        assertEquals(1, groups.size)
        assertEquals(3, groups.first().size)
    }

    @Test
    fun `alice docs folder has no duplicates`() {
        val groups = deduplicator.findDuplicateGroups("alice", "/docs")
        assertEquals(0, groups.size)
    }

    @Test
    fun `bob has no duplicates within his files`() {
        val groups = deduplicator.findDuplicateGroups("bob", "/")
        assertEquals(0, groups.size)
    }

    @Test
    fun `findDuplicate finds duplicate for indexed file`() {
        val path = Path.of(ClassLoader.getSystemResource("filesystem/bob/a.txt").toURI())
        assertNotNull(deduplicator.findDuplicate(path))
    }

    @Test
    fun `findDuplicate returns null for unique indexed file`() {
        val path = Path.of(ClassLoader.getSystemResource("filesystem/bob/b.txt").toURI())
        assertNull(deduplicator.findDuplicate(path))
    }

    @Test
    fun `findDuplicate finds duplicate for new file not in index`() {
        val tmp = Files.createTempFile("test", ".txt").also { Files.writeString(it, "a") }
        try {
            assertNotNull(deduplicator.findDuplicate(tmp))
        } finally {
            Files.delete(tmp)
        }
    }

    @Test
    fun `findDuplicate returns null for new unique file`() {
        val tmp = Files.createTempFile("test", ".txt").also { Files.writeString(it, "unique_content") }
        try {
            assertNull(deduplicator.findDuplicate(tmp))
        } finally {
            Files.delete(tmp)
        }
    }
}
