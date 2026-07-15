group = "com.neclor"

plugins {
    alias(libs.plugins.kotlin.jvm)
    alias(libs.plugins.shadow)
}

repositories {
    mavenCentral()
    maven {
        url = uri("https://gitlab.uliege.be/api/v4/projects/8360/packages/maven")
        name = "GitLab ULiège"
        credentials(HttpHeaderCredentials::class) {
            name = findProperty("gitLabTokenName") as String?
            value = findProperty("gitLabPrivateToken") as String?
        }
        authentication {
            create("header", HttpHeaderAuthentication::class)
        }
    }
}

kotlin {
    jvmToolchain(25)
}

dependencies {
    implementation(libs.filededup.interfaces)
    implementation(libs.kotlinx.serialization.json)
    implementation(libs.opencv)

    testImplementation(libs.kotlin.test)
}

tasks.shadowJar {
    archiveFileName = "Deduplicator.jar"
}

tasks.build {
    dependsOn(tasks.shadowJar)
}

tasks.test {
    useJUnitPlatform()
    jvmArgs("--enable-native-access=ALL-UNNAMED")
}
