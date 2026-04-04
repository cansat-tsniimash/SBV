import serial # импортируем библиотеку и фаил с основным кодом
import madgic
from madgic import Parser, convert

port = serial.Serial("COM4", baudrate=9600) # выбираем порт и скорость данных по юарту
port.timeout = 1
p = madgic.Parser()

tim = "Tim ID"
time = "Время"
c = "Температура"
d = "Давление"
vx = "Ускорение по x"
vy = "Ускорение по y"
vz = "Ускорение по z"
xx = "Угловая скорость по x"
xy = "Угловая скорость по y"
xz = "Угловая скорость по z"
#ввожу переменые для названий заголовков столбов

with open("telem.bin", mode="wb") as stream, open("chtoto.csv", mode="w") as g : # записывает чтото там
    self = b""
    while True:
        data = port.read(4096)
        stream.write(data)
        packets = p.parse(data)
        for packet in packets:
            self = self + data
            while len(self) > 27:
                values, leftovers = Parser(self)
                numbers = convert(values)
            g.write(";".join(str(x).replace(".", ",") for x in numbers + "\n")) #меняю точки на запетые для иксель
            print(tim, time, c, d, vx, vy, vz, xx, xy, xz, "\n", numbers) # записываем отредоктированые пакеты
            self = leftovers
