import be.uliege.info0027.deduplication.*
import java.nio.file.*
import kotlin.io.path.*
import kotlin.text.removePrefix

class TestFileSystem : VirtualFileSystem {

    private val root = Path.of(ClassLoader.getSystemResource("filesystem").toURI())

    private val entries: List<VirtualFileInfo> = Files.walk(root)
        .filter { path -> path != root }
        .map { path ->
            val userId = root.relativize(path).first().toString()
            val userRoot = root.resolve(userId)
            val virtualPath = "/" + path.relativeTo(userRoot).toString().replace("\\", "/")
            VirtualFileInfo(
                virtualPath,
                path.isDirectory(),
                path.fileSize(),
                userId,
                path.toString()
            )
        }
        .toList()

    override fun listContent(): List<VirtualFileInfo> = entries.filter { info -> info.virtualPath == "/" }

    override fun listContent(userId: String, virtualPath: String): List<VirtualFileInfo> =
        entries.filter { info ->
            if (info.userId != userId) return@filter false

            val parentPath = if (virtualPath.endsWith("/")) virtualPath else "$virtualPath/"
            if (!info.virtualPath.startsWith(parentPath)) return@filter false

            val suffix = info.virtualPath.removePrefix(parentPath)
            return@filter suffix.isNotEmpty() && !suffix.contains("/")
        }
}
