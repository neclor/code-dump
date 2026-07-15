import wasi.readln

fun main() {
    println("WASM ECHO")
    while (true) {
        print("> ")
        println("Wasm received: " + readln())
    }
}
