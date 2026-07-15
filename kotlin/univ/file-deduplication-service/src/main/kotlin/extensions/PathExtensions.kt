package extensions

import java.nio.file.Files
import java.nio.file.Path
import java.security.MessageDigest

private const val BUFFER_SIZE = 8192

/** Returns the SHA-256 hex digest of this file's contents. */
fun Path.sha256(): String {
    val digest = MessageDigest.getInstance("SHA-256")

    Files.newInputStream(this).use { inputStream ->
        val buffer = ByteArray(BUFFER_SIZE)
        var bytesRead: Int

        while (true) {
            bytesRead = inputStream.read(buffer)
            if (bytesRead == -1) break

            digest.update(buffer, 0, bytesRead)
        }
    }
    return digest.digest().joinToString("") { "%02x".format(it) }
}

/** Returns true if this file and [other] have identical size and byte contents. */
fun Path.isEqual(other: Path): Boolean {
    if (Files.size(this) != Files.size(other)) return false

    Files.newInputStream(this).use { inputStreamA ->
        Files.newInputStream(other).use { inputStreamB ->
            val bufferA = ByteArray(BUFFER_SIZE)
            val bufferB = ByteArray(BUFFER_SIZE)
            var bytesReadA: Int
            var bytesReadB: Int

            do {
                bytesReadA = inputStreamA.readNBytes(bufferA, 0, BUFFER_SIZE)
                bytesReadB = inputStreamB.readNBytes(bufferB, 0, BUFFER_SIZE)
                if (bytesReadA != bytesReadB || !bufferA.contentEquals(bufferB)) return false
            } while (bytesReadA == BUFFER_SIZE)
            return true
        }
    }
}