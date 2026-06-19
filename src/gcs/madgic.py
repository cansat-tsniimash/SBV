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

def lis2mdl_from_lsd_to_mgauss(lsb: int) -> float:
    return lsb * 1.5
# какиета нужные функции для акселерометра и магнетометра

def convert_std(values):
    # return values
    return (
        values[1],
        values[2],
        values[3] / 100,
        values[4],
        lsm6ds3_from_fs16g_to_mg(values[5]) / 1000,
        lsm6ds3_from_fs16g_to_mg(values[6]) / 1000,
        lsm6ds3_from_fs16g_to_mg(values[7]) / 1000,
        lsm6ds3_from_fs2000dps_to_mdps(values[8]) / 1000,
        lsm6ds3_from_fs2000dps_to_mdps(values[9]) / 1000,
        lsm6ds3_from_fs2000dps_to_mdps(values[10]) / 1000,
    )
# функцией превращаем попугаев в нужные еденицы измерения части аргонизаторов


def convert_our(values):
    # return values
    return (
        values[0],
        values[1],
        values[2],
        values[3],
        values[4],
        values[5],
        values[6] / 1000,
        values[7] / 1000,
        values[8] / 1000,
        values[9] / 1000,
        values[10] / 1000,
        values[11] / 1000,
        values[12],
        values[13],
        values[14],
        values[15],
        lis2mdl_from_lsd_to_mgauss(values[16]),
        lis2mdl_from_lsd_to_mgauss(values[17]),
        lis2mdl_from_lsd_to_mgauss(values[18]),
        values[19] / 10,
        lsm6ds3_from_fs2000dps_to_mdps(values[20]) / 1000,
        lsm6ds3_from_fs2000dps_to_mdps(values[21]) / 1000,
        lsm6ds3_from_fs2000dps_to_mdps(values[22]) / 1000,
        lsm6ds3_from_fs16g_to_mg(values[23]) / 1000,
        lsm6ds3_from_fs16g_to_mg(values[24]) / 1000,
        lsm6ds3_from_fs16g_to_mg(values[25]) / 1000,
        values[26],
    )
# функцией превращаем попугаев в нужные еденицы измерения части

class Parser: # подклас библеотеки
    STRUCT_STD = struct.Struct("<HHIhI3h3hB")
    STRUCT_OUR = struct.Struct("<HB3fB6H2h2h10hfB")

    def __init__(self):
        self.lef = bytes()

    #функция превращающая данные в биты
    def parse(self, data: bytes): # преврашение даты в пакпет
        rv = []
        data = self.lef + data

        while len(data) >= self.STRUCT_STD.size + self.STRUCT_OUR.size: # если не достаточно данных то не работает
            while len(data) > 2 and (data[0] != 0xaa or data[1] != 0xaa):
                data = data[1:]
            # находим метку начала пакета
            if len(data) < self.STRUCT_STD.size + self.STRUCT_OUR.size:
                break

            packet = data[:self.STRUCT_STD.size] # отсикаем метку начала пакета
            chk = packet[0]
            for b in packet[1:]:
                chk = chk ^ b
            if chk == 0: # сверяем контрольную сумму
                values = self.STRUCT_STD.unpack(packet) # рашифровываем основную часть покета
                data = data[self.STRUCT_STD.size:] # перенсим в нашу часть покета
                copi = data[0:self.STRUCT_OUR.size]
                chk = copi[0]
                for o in copi[1:]:
                    chk = chk ^ o
                if chk == 0: # контрольная сумма нашей части пакета
                    orig = self.STRUCT_OUR.unpack(copi) # рашифровка нашей части пакета
                    rv.append((values, orig,)) # записваем в масив
                    data = data[self.STRUCT_OUR.size:] # берём другую часть пакета
                else: # если что то пошло не так
                    rv.append((values, None,))  # записваем в масив
                    data = data[1:]
            else: # если что то пошло не так
                rv.append((None, None,))
                data = data[1:]

        self.lef = data
        return rv # возвращаем масив с пакетами

headers_std = ("Tim ID", "   time", " temp", "     P", "   a[x]", "  a[y]", "  a[z]", " w[x]", " w[y]", " w[z]",)
headers_our = (
    "  num", "st", "   lat", "   lon", "     h", "  fx",
    "  ph[0]", "  ph[1]", "  ph[2]", "  ph[3]", "  ph[4]", "  ph[5]",
    " bus1", "shunt1", " bus2", "shunt2",
    " mag[x]", " mag[y]", " mag[z]", " temp", "  a2[x]", "  a2[y]", "  a2[z]", " w2[x]", " w2[y]", " w2[z]", "y_p"
)

def print_aligned(headers, values):
#    print(*headers)
    for no, header in enumerate(headers):
        value = values[no]
        if isinstance(value, float):
            value = "%.2f" % value
        else:
            value = str(value)

        value = " " * (len(header) - len(value)) + value
        print(value, end=" ")

    print()
