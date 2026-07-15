group = "com.neclor"
version = "1.0-SNAPSHOT"

plugins {
    alias(libs.plugins.kotlin.jvm)
    application
}

application {
    mainClass = "MainKt"
}

repositories {
    mavenCentral()
}

kotlin {
    jvmToolchain(21)
}

dependencies {
    implementation(libs.ktor.client.core)
    implementation(libs.ktor.client.cio)
    implementation(libs.clikt)
    implementation(libs.logback)

    testImplementation(libs.ktor.client.mock)
    testImplementation(libs.kotlin.test)
}

tasks.test {
    useJUnitPlatform()
}
