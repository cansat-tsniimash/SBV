import struct # импортируем библиотеку

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
# какиета нужные функции

def convert_std(values):
    return (
        values[1],
        values[2],
        values[3] / 100,
        values[4],
        lsm6ds3_from_fs16g_to_mg(values[5]),
        lsm6ds3_from_fs16g_to_mg(values[6]),
        lsm6ds3_from_fs16g_to_mg(values[7]),
        lsm6ds3_from_fs2000dps_to_mdps(values[8]),
        lsm6ds3_from_fs2000dps_to_mdps(values[9]),
        lsm6ds3_from_fs2000dps_to_mdps(values[10]),
    )
# превращаем попугаев в нужные еденицы измерения


def convert_our(values):
    return (
        values[0],
        values[1],
        values[2],
        values[3],
        values[4],
        values[5],
        values[6],
        values[7],
        values[8],
        values[9],
        values[10],
        values[11],
        values[12],
        values[13],
        values[14],
        values[15],
        values[16] / 10,
        lsm6ds3_from_fs2000dps_to_mdps(values[17]),
        lsm6ds3_from_fs2000dps_to_mdps(values[18]),
        lsm6ds3_from_fs2000dps_to_mdps(values[19]),
        lsm6ds3_from_fs16g_to_mg(values[20]),
        lsm6ds3_from_fs16g_to_mg(values[21]),
        lsm6ds3_from_fs16g_to_mg(values[22]),
    )

class Parser: # подклас библеотеки
    def __init__(self):
        self.lef = bytes()

    #функция превращающая данные в биты
    def parse(self, data: bytes): # преврашение даты в пакпет
        rv = []
        data = self.lef + data

        while len(data) >= 81: # если не достаточно данных то не работает
            while len(data) > 2 and (data[0] != 0xaa or data[1] != 0xaa):
                data = data[1:]
            # находим метку начала пакета
            if len(data) < 81:
                break

            packet = data[:27] # отсикаем метку начала пакета
            chk = packet[0]
            for b in packet[1:]:
                chk = chk ^ b
            if chk == 0: # сверяем контрольную сумму
                values = struct.unpack("<HHIhI3h3hB", packet) # рашифровываем основную часть покета
                data = data[28:] # перенсим в нашу часть покета
                copi = data[0:53]
                chk = copi[0]
                for o in copi[1:]:
                    chk = chk ^ o
                if chk == 0: # контрольная сумма нашей части пакета
                    orig = struct.unpack("<HB3fB6Hf10HB", copi) # рашифровка нашей части пакета
                    rv.append((values, orig,)) # записваем в масив
                    data = data[54:] # берём другую часть пакета
                else: # если что то пошло не так
                    rv.append((values, None,))  # записваем в масив
                    data = data[1:]
            else: # если что то пошло не так
                rv.append((None, None,))
                data = data[1:]

        self.lef = data
        return rv # возвращаем масив с пакетами
