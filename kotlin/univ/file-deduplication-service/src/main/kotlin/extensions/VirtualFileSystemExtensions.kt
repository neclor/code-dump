package extensions

import be.uliege.info0027.deduplication.VirtualFileInfo
import be.uliege.info0027.deduplication.VirtualFileSystem

/** Returns all files (non-directories) across all users in the VFS. */
fun VirtualFileSystem.getAllFiles(): Collection<VirtualFileInfo> =
    listContent().flatMap { userRoot ->
        getUserFiles(userRoot.userId, userRoot.virtualPath)
    }

/** Recursively returns all files (non-directories) under [virtualPath] for [userId]. */
fun VirtualFileSystem.getUserFiles(userId: String, virtualPath: String): Collection<VirtualFileInfo> =
    listContent(userId, virtualPath).flatMap { entry ->
        if (entry.isDirectory == true) {
            getUserFiles(userId, entry.virtualPath)
        } else {
            listOf(entry)
        }
    }
