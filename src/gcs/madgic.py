import struct

def lsm6ds3_from_fs2g_to_mg(lsb: int) -> float:
  return lsb * 61.0 / 1000.0

def lsm6ds3_from_fs4g_to_mg(lsb: int) -> float:
  return lsb * 122 / 1000.0

def lsm6ds3_from_fs8g_to_mg(lsb: int) -> float:
  return lsb * 244.0 / 1000.0

def lsm6ds3_from_fs16g_to_mg(lsb: int) -> float:
  return lsb * 488.0 / 1000.0

def lsm6ds3_from_fs125dps_to_mdps(lsb: int) -> float:
  return lsb * 4375.0 / 1000.0

def lsm6ds3_from_fs250dps_to_mdps(lsb: int) -> float:
  return lsb * 8750.0 / 1000.0

def lsm6ds3_from_fs500dps_to_mdps(lsb: int) -> float:
  return lsb * 1750.0 / 1000.0

def lsm6ds3_from_fs1000dps_to_mdps(lsb: int) -> float:
  return lsb * 35.0

def lsm6ds3_from_fs2000dps_to_mdps(lsb: int) -> float:
  return lsb * 70.0


def convert(values):
    values[0],
    values[1],
    values[2] / 16,
    values[3],
    lsm6ds3_from_fs16g_to_mg(values[4]),
    lsm6ds3_from_fs16g_to_mg(values[5]),
    lsm6ds3_from_fs16g_to_mg(values[6]),
    lsm6ds3_from_fs2000dps_to_mdps(values[7]),
    lsm6ds3_from_fs2000dps_to_mdps(values[8]),
    lsm6ds3_from_fs2000dps_to_mdps(values[9]),

class Parser:
    def __init__(self):
        self.lef = bytes()

    def parse(self, data: bytes):
        rv = []
        hz = []
        data = self.lef + data

        while len(data) >= 76:
            while len(data) > 2 and (data[0] != 0xaa or data[1] != 0xaa):
                data = data[1:]

            if len(data) < 76:
                break

            packet = data[2:27]
            chk = packet[0]
            for b in packet[1:]:
                chk = chk ^ b
            if chk == 0:
                values = struct.unpack("<HIhI3h3hB", packet)
                rv.append(values)
                data = data[27:]
                copi = data[0:50]
                sym = copi[0]
                for o in copi [1:]:
                    copi = copi ^ o
                if copi == 0:
                    orig = struct.unpack("<B3IB6H2HhB", packet)
                    hz.append(orig)
                    data = data[50:]
                else:
                    rv.append(None)
                    hz.append(None)
                    data = data[1:]
            else:
                rv.append(None)
                hz.append(None)
                data = data[1:]

        self.lef = data
        return rv, hz
