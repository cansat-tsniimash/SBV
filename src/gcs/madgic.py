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

def convert(values):
    return (
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
    )
# превращаем попугаев в нужные еденицы измерения

class Parser: # подклас библеотеки
    def __init__(self):
        self.lef = bytes()

    #функция превращающая данные в биты
    def parse(self, data: bytes): # преврашение даты в пакпет
        rv = []
        hz = [] # формирования масивов для записи частей пакета
        data = self.lef + data

        while len(data) >= 77: # если не достаточно данных то не работает
            while len(data) > 2 and (data[0] != 0xaa or data[1] != 0xaa):
                data = data[1:]
            # находим метку начала пакета
            if len(data) < 77:
                break

            packet = data[2:27] # отсикаем метку начала пакета и тим айди
            chk = packet[0]
            for b in packet[1:]:
                chk = chk ^ b
            if chk == 0: # сверяем контрольную сумму
                values = struct.unpack("<HIhI3h3hB", packet) # рашифровываем основную часть покета
                rv.append(values) # записываем в масив
                data = data[28:] # перенсим в нашу часть покета
                copi = packet[0:50]
                for o in packet [1:]:
                    copi = copi ^ o
                if copi == 0: # контрольная сумма нашей части пакета
                    orig = struct.unpack("<B3IB6H2HhB", packet) # рашифровка нашей части пакета
                    hz.append(orig) # записваем в масив
                    data = data[50:] # берём другую часть пакета
                else: # если что то пошло не так
                    hz.append(None)
                    data = data[1:]
            else: # если что то пошло не так
                rv.append(None)
                hz.append(None)
                data = data[1:]

        self.lef = data
        return rv, hz # возвращаем масивы с пакетами
