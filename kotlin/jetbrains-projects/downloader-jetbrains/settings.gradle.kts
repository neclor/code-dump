rootProject.name = "downloader-jetbrains"

pluginManagement {
    resolutionStrategy {
        repositories {
            gradlePluginPortal()
        }
    }
}

plugins {
    id("org.gradle.toolchains.foojay-resolver-convention") version "1.0.0"
}
