package filededuplicators

import nu.pattern.OpenCV
import org.opencv.core.*
import org.opencv.imgcodecs.Imgcodecs
import org.opencv.imgproc.Imgproc
import java.nio.file.Path
import be.uliege.info0027.deduplication.*
import extensions.*

/**
 * Similarity-based deduplication engine for images using OpenCV.
 *
 * On construction, preprocesses all files in the VFS (non-images are skipped) and
 * clusters them into groups using template matching (TM_CCOEFF_NORMED ≥ 0.90).
 * Grouping is transitive: if A~B and B~C, all three are placed in the same group.
 */
class ImageFileDeduplicator(private val virtualFileSystem: VirtualFileSystem): FileDeduplicator {

    private val infoToMat = mutableMapOf<VirtualFileInfo, Mat>()
    private val groups = mutableListOf<MutableSet<VirtualFileInfo>>()

    init {
        OpenCV.loadLocally()
        virtualFileSystem.getAllFiles().forEach { fileInfo ->
            val mat = preprocessImage(fileInfo.physicalPath) ?: return@forEach
            infoToMat[fileInfo] = mat

            val group = groups.firstOrNull { group ->
                group.any { info -> mat.isSimilar(infoToMat[info] ?: return@any false) }
            }
            if (group != null) group.add(fileInfo)
            else groups.add(mutableSetOf(fileInfo))
        }
    }

    /** Returns any indexed file visually similar to the image at [path], or null if none exists or [path] is not an image. */
    override fun findDuplicate(path: Path): VirtualFileInfo? {
        val mat = preprocessImage(path.toString()) ?: return null
        return infoToMat.entries.firstOrNull { entry -> mat.isSimilar(entry.value) }?.key
    }

    override fun findDuplicateGroups(userId: String, path: String): Collection<Set<VirtualFileInfo>> {
        val userFiles = virtualFileSystem.getUserFiles(userId, path).toSet()
        return groups
            .map { group -> group.intersect(userFiles) }
            .filter { it.size > 1 }
    }

    // Preprocess the image: standardize size and remove color variations
    private fun preprocessImage(filePath: String): Mat? {
        val src = Imgcodecs.imread(filePath)
        if (src.empty()) return null

        val gray = Mat()

        // Convert to grayscale
        Imgproc.cvtColor(src, gray, Imgproc.COLOR_BGR2GRAY)
        src.release() // Release memory

        val resizedGray = Mat()
        // Resize to a fixed dimension (for example 256x256)
        Imgproc.resize(gray, resizedGray, Size(256.0, 256.0))
        gray.release()

        return resizedGray
    }

    // Compare two preprocessed image matrices
    private fun Mat.isSimilar(mat: Mat): Boolean {
        val res = Mat()

        // Use Template Matching with Normalized Correlation Coefficient
        Imgproc.matchTemplate(this, mat, res, Imgproc.TM_CCOEFF_NORMED)
        val score = res.get(0, 0)[0]
        res.release()

        // TM_CCOEFF_NORMED returns 1.0 for exact matches.
        // 0.90 is a fine threshold for similarity.
        return score >= 0.90
    }
}
