import serial # импортируем библиотеку и фаил с основным кодом
import madgic
import datetime
from madgic import convert_std, convert_our, headers_std, headers_our, print_aligned

postfix = datetime.datetime.now().isoformat().replace(":", "-")
port = serial.Serial("COM4", baudrate=9600) # выбираем порт и скорость данных по юарту
port.timeout = 0.05
p = madgic.Parser()
#ввожу переменые для названий заголовков столбов
print(" ".join(headers_std + headers_our))
with open("telem" + postfix + ".bin", mode="wb") as stream, open("chtoto.csv", mode="w") as g : # записывает чтото там
    while True:
        data = port.read(4096)
        stream.write(data)
        stream.flush()
        packets = p.parse(data)
        for std, our in packets:
            if std is None:
                print("BAD STD CHECKSUM!")
                continue

            numbers = convert_std(std)
            #g.write((";".join(str(x).replace(".", ",") for x in numbers) + "\n")) #меняю точки на запетые для иксель
            if our is None:
                print( "BAD OUR CHECKSUM")
                print_aligned(headers_std, numbers)
                continue

            numbers2 = convert_our(our)
            print_aligned(headers_std + headers_our, numbers + numbers2)
