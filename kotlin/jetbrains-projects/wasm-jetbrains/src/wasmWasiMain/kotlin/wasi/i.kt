@file:OptIn(ExperimentalWasmInterop::class, UnsafeWasmMemoryApi::class)

package wasi

import kotlin.wasm.WasmImport
import kotlin.wasm.ExperimentalWasmInterop
import kotlin.wasm.unsafe.MemoryAllocator
import kotlin.wasm.unsafe.Pointer
import kotlin.wasm.unsafe.UnsafeWasmMemoryApi
import kotlin.wasm.unsafe.withScopedMemoryAllocator

private fun Pointer.toWasiPointer(): WasiPointer = this.address
private fun Pointer.loadUInt(): UInt = this.loadInt().toUInt()
private fun Pointer.storeUInt(value: UInt): Unit = this.storeInt(value.toInt())

private typealias WasiPointer = UInt
private typealias WasiSize = UInt
private typealias WasiFd = UInt

private const val IOVEC_SIZE = 8
private const val IOVEC_BUF_OFFSET = 0
private const val IOVEC_BUF_LEN_OFFSET = 4

private const val SUBSCRIPTION_SIZE = 48
private const val SUBSCRIPTION_USER_DATA_OFFSET = 0
private const val SUBSCRIPTION_U_OFFSET = 8
private const val SUBSCRIPTION_U_TAG_OFFSET = 0
private const val SUBSCRIPTION_U_DATA_OFFSET = 8
private const val SUBSCRIPTION_FD_READWRITE_FD_OFFSET = 0

private const val EVENT_TYPE_FD_READ: Byte = 1

private const val EVENT_SIZE = 32
private const val EVENT_USER_DATA_OFFSET = 0
private const val EVENT_ERROR_OFFSET = 8
private const val EVENT_TYPE_OFFSET = 10

private const val USER_DATA = 0L

private const val STDIN: WasiFd = 0u

private const val CR: Byte = 0x0D.toByte()
private const val LF: Byte = 0x0A.toByte()

fun readln(): String {
    withScopedMemoryAllocator { allocator ->
        // poll_oneoff on stdin fails with error 57 (ENOTSOCK) in Node.js on Windows
        waitInput(allocator)
        return readLine(allocator)
    }
}

private fun waitInput(allocator: MemoryAllocator) {
    val subscriptionPointer: Pointer = createSubscriptionRead(allocator, USER_DATA)
    val eventPointer: Pointer = createEvent(allocator)

    val errorPointer: Pointer = allocator.allocate(WasiSize.SIZE_BYTES)
    val result: Int = wasiPollOneOff(
        subscriptionPointer.toWasiPointer(),
        eventPointer.toWasiPointer(),
        1U,
        errorPointer.toWasiPointer()
    )
    check(result == 0) { "WasiError: $result" }
    check(errorPointer.loadUInt() == 1U) { "Unexpected WASI result" }

    check((eventPointer + EVENT_USER_DATA_OFFSET).loadLong() == USER_DATA) { "Unexpected WASI result" }

    val eventResult: Short = (eventPointer + EVENT_ERROR_OFFSET).loadShort()
    check(eventResult.toInt() == 0) { "WasiError: EventError $eventResult" }

    val eventType: Byte = (eventPointer + EVENT_TYPE_OFFSET).loadByte()
    check(eventType == EVENT_TYPE_FD_READ) { "Unexpected WASI result" }
}

private fun readLine(allocator: MemoryAllocator): String {
    val bytes = mutableListOf<Byte>()

    val bufPointer: Pointer = allocator.allocate(1)
    val iovecPointer: Pointer = createIovec(allocator, bufPointer.toWasiPointer(), 1U)

    val errorPointer: Pointer = allocator.allocate(WasiSize.SIZE_BYTES)
    while (true) {
        val result: Int = wasiRawFdRead(
            STDIN,
            iovecPointer.toWasiPointer(),
            1U,
            errorPointer.toWasiPointer()
        )
        check(result == 0) { "WasiError: $result" }
        val readSize: UInt = errorPointer.loadUInt()
        check(readSize == 0U || readSize == 1U) { "Unexpected WASI result" }

        if (readSize == 0U && bytes.isEmpty()) throw RuntimeException("EOF has already been reached")

        val nextByte: Byte = bufPointer.loadByte()
        if (readSize == 0U || nextByte == LF) {
            if (bytes.lastOrNull() == CR) {
                bytes.removeLast()
            }
            return bytes.toByteArray().decodeToString()
        }

        bytes += nextByte
    }
}

private fun createIovec(allocator: MemoryAllocator, buf: WasiPointer, bufLen: WasiSize): Pointer {
    val iovecPointer: Pointer = allocator.allocate(IOVEC_SIZE)
    (iovecPointer + IOVEC_BUF_OFFSET).storeUInt(buf)
    (iovecPointer + IOVEC_BUF_LEN_OFFSET).storeUInt(bufLen)
    return iovecPointer
}

private fun createSubscriptionRead(allocator: MemoryAllocator, userData: Long): Pointer {
    val subscriptionPointer: Pointer = allocator.allocate(SUBSCRIPTION_SIZE)
    (subscriptionPointer + SUBSCRIPTION_USER_DATA_OFFSET).storeLong(userData)
    (subscriptionPointer + SUBSCRIPTION_U_OFFSET + SUBSCRIPTION_U_TAG_OFFSET).storeByte(EVENT_TYPE_FD_READ)
    (subscriptionPointer + SUBSCRIPTION_U_OFFSET + SUBSCRIPTION_U_DATA_OFFSET + SUBSCRIPTION_FD_READWRITE_FD_OFFSET).storeUInt(STDIN)
    return subscriptionPointer
}

private fun createEvent(allocator: MemoryAllocator): Pointer {
    val eventPointer: Pointer = allocator.allocate(EVENT_SIZE)
    return eventPointer
}

@WasmImport("wasi_snapshot_preview1", "poll_oneoff")
private external fun wasiPollOneOff(inSubscription: WasiPointer, outEvent: WasiPointer, nSubscriptions: WasiSize, error: WasiPointer): Int

@WasmImport("wasi_snapshot_preview1", "fd_read")
private external fun wasiRawFdRead(fd: WasiFd, iovsIovecArrayPointer: WasiPointer, iovsIovecArraySize: WasiSize, error: WasiPointer): Int
