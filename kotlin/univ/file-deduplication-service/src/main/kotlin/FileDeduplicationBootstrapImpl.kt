import be.uliege.info0027.deduplication.*
import filededuplicators.*

/**
 * Service entry point discovered via [java.util.ServiceLoader].
 * Wires together the VFS, deduplication engines, and the frontend gate.
 */
class FileDeduplicationBootstrapImpl : FileDeduplicationBootstrap {

    private var frontendGate: FrontendGate? = null
    private var exactFileDeduplicator: FileDeduplicator? = null
    private var similarFileDeduplicator: FileDeduplicator? = null

    override fun initialize(fileSystem: VirtualFileSystem) {
        exactFileDeduplicator = KotlinFileDeduplicator(fileSystem)
        similarFileDeduplicator = ImageFileDeduplicator(fileSystem)
        frontendGate = FrontendGateImpl(exactFileDeduplicator!!, similarFileDeduplicator!!)
    }

    override fun getFrontendGate(): FrontendGate? = frontendGate

    override fun getStorageChecker(): StorageChecker? = exactFileDeduplicator
}
