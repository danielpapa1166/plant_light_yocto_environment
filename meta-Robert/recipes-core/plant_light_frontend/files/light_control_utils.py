import struct

def float_to_u8_array_le(value: float) -> bytes:
    return struct.pack('<f', value)


def u8_array_to_float_le(data: bytes) -> float:
    if len(data) != 4:
        raise ValueError("Expected exactly 4 bytes")
    return struct.unpack('<f', data)[0]
