package filededuplicators

import java.nio.file.Path
import be.uliege.info0027.deduplication.*
import extensions.*

/**
 * Exact deduplication engine using SHA-256 hashing.
 *
 * On construction, indexes all files in the VFS. Files with the same hash are
 * grouped; byte-level equality ([Path.isEqual]) is used to handle hash collisions.
 * The index structure is `hash → list of physically-identical groups`.
 */
class KotlinFileDeduplicator(private val virtualFileSystem: VirtualFileSystem) : FileDeduplicator {

    private val hashGroups = mutableMapOf<String, MutableList<MutableSet<VirtualFileInfo>>>()

    private val infoToHash = mutableMapOf<VirtualFileInfo, String>()
    private val pathToInfo = mutableMapOf<Path, VirtualFileInfo>()

    init {
        virtualFileSystem.getAllFiles().forEach { fileInfo ->
            val physicalPath = Path.of(fileInfo.physicalPath)
            pathToInfo[physicalPath] = fileInfo

            val hash = physicalPath.sha256()
            infoToHash[fileInfo] = hash

            val groups = hashGroups.getOrPut(hash) { mutableListOf() }
            val group = groups.firstOrNull { group ->
                physicalPath.isEqual(Path.of(group.first().physicalPath))
            }
            if (group == null) groups.add(mutableSetOf(fileInfo))
            else group.add(fileInfo)
        }
    }

    /** Returns any indexed file that is an exact duplicate of [path], or null if none exists. */
    override fun findDuplicate(path: Path): VirtualFileInfo? {
        val fileInfo = pathToInfo[path]
        if (fileInfo != null) {
            val hash = infoToHash[fileInfo] ?: return null
            val group = hashGroups[hash]?.firstOrNull { group -> group.contains(fileInfo) } ?: return null
            return group.firstOrNull { Path.of(it.physicalPath) != path }
        }

        val hash = path.sha256()
        val group = hashGroups[hash]?.firstOrNull { group ->
            path.isEqual(Path.of(group.first().physicalPath))
        } ?: return null
        return group.first()
    }

    override fun findDuplicateGroups(userId: String, path: String): Collection<Set<VirtualFileInfo>> {
        val userFiles = virtualFileSystem.getUserFiles(userId, path).toSet()

        return hashGroups.values
            .flatten()
            .map { group -> group.intersect(userFiles) }
            .filter { it.size > 1 }
    }
}
