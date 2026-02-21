import struct

class Parser:
    def __init__(self):
        self.lef = bytes()

    def parse(self, data: bytes):
        rv = []
        data = self.lef + data

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
                rv.append(values)
                data = data[27:]
            else:
                rv.append(None)
                data = data[1:]

        self.lef = data
        return rv
