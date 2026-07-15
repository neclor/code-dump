package filededuplicators

import be.uliege.info0027.deduplication.*

/**
 * Deduplication engine that can scan a user's files for duplicates.
 * Extends [StorageChecker] to also support on-the-fly duplicate detection by physical path.
 */
interface FileDeduplicator : StorageChecker {

    /**
     * Returns groups of duplicate files visible to [userId] under [path].
     * Each group contains at least two files considered identical or similar.
     */
    fun findDuplicateGroups(userId: String, path: String): Collection<Set<VirtualFileInfo>>
}
