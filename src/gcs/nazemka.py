import serial
import struct

port = serial.Serial("COM4", baudrate=115200)
port.timeout = 1

data = bytes()
while True:
    data += port.read(4096)

    while len(data) >= 27:
        while len(data) > 2 and (data[0] != 0xaa or data[1] != 0xaa):
            data = data[1:]

        if len(data) < 27:
            break

        packet = data[2:27]
        chk = packet[0]
        for b in packet[1:]:
            chk = chk ^ b
        if chk == 0:
            values = struct.unpack("<HIhI3h3hB", packet)
            print(values)
            data = data[27:]
        else:
            print("BAD PACKET!")
            data = data[1:]
