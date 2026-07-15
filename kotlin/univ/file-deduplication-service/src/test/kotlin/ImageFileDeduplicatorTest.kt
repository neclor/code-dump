import filededuplicators.ImageFileDeduplicator
import java.nio.file.Path
import kotlin.test.*

class ImageFileDeduplicatorTest {

    private val fileSystem = TestFileSystem()
    private val deduplicator = ImageFileDeduplicator(fileSystem)

    @Test
    fun `alice has one duplicate image group`() {
        val groups = deduplicator.findDuplicateGroups("alice", "/")
        assertEquals(1, groups.size)
        assertEquals(2, groups.first().size)
    }

    @Test
    fun `bob has no duplicate image groups`() {
        val groups = deduplicator.findDuplicateGroups("bob", "/")
        assertEquals(0, groups.size)
    }

    @Test
    fun `findDuplicate finds similar image from another user`() {
        val path = Path.of(ClassLoader.getSystemResource("filesystem/bob/images/image_a.png").toURI())
        assertNotNull(deduplicator.findDuplicate(path))
    }

    @Test
    fun `findDuplicate finds similar image within same user`() {
        val path = Path.of(ClassLoader.getSystemResource("filesystem/alice/images/image_a_similar.png").toURI())
        assertNotNull(deduplicator.findDuplicate(path))
    }

    @Test
    fun `findDuplicate returns null for non-image file`() {
        val path = Path.of(ClassLoader.getSystemResource("filesystem/alice/a.txt").toURI())
        assertNull(deduplicator.findDuplicate(path))
    }
}
