import serial # импортируем библиотеку и фаил с основным кодом
import madgic

port = serial.Serial("COM4", baudrate=115200) # выбираем порт и скорость данных по юарту
port.timeout = 1
p = madgic.Parser()

with open("telem.bin", mode="wb") as stream, open("chtoto.csv", mode="w") as g : # записывает чтото там
    while True:
        data = port.read(4096)
        stream.write(data)
        packets = p.parse(data)
        for packet in packets:
            numbers = madgic.convert()
            g.write(",".join("."))
            # меняем точки на запетые для иксель таблиц
            print(numbers) # записываем отредоктированые пакеты
