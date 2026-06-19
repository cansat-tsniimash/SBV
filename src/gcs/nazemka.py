import serial # импортируем библиотеку и фаил с основным кодом
import madgic
from madgic import Parser, convert_std, convert_our

headers_std = ("Tim ID", "   time", " temp", "     P", "   a[x]", "  a[y]", "  a[z]", " w[x]", " w[y]", " w[z]",)
headers_our = (
    "  num", "st", "   lat", "   lon", "     h", "  fx",
    "  ph[0]", "  ph[1]", "  ph[2]", "  ph[3]", "  ph[4]", "  ph[5]",
    " bus1", "shunt1", " bus2", "shunt2",
    " mag[x]", " mag[y]", " mag[z]", " temp", "  a2[x]", "  a2[y]", "  a2[z]", " w2[x]", " w2[y]", " w2[z]", "y_p"
)
#ввожу переменые для названий заголовков столбов
print(headers_std, headers_our, sep="")

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

port = serial.Serial("COM4", baudrate=9600) # выбираем порт и скорость данных по юарту
port.timeout = 0.05
p = madgic.Parser()

with open("telem.bin", mode="wb") as stream, open("chtoto.csv", mode="w") as g : # записывает чтото там
    while True:
        data = port.read(4096)
        stream.write(data)
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
