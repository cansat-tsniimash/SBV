import serial # импортируем библиотеку и фаил с основным кодом
import madgic
from madgic import convert_std, convert_our, headers_std, headers_our, print_aligned

p = madgic.Parser()
#ввожу переменые для названий заголовков столбов
print(" ".join(headers_std + headers_our))
with open("SDPack.bin", mode="rb") as stream, open("chtoto2.csv", mode="w") as g : # записывает чтото там
    g.write(";".join(x.strip() for x in headers_std + headers_our) + "\n")
    while True:
        data = stream.read(4096)
        if not data:
            break

        packets = p.parse(data)
        for std, our in packets:
            if std is None:
                print("BAD STD CHECKSUM!")
                continue

            numbers = convert_std(std)
            if our is None:
                print( "BAD OUR CHECKSUM")
                print_aligned(headers_std, numbers)
                # меняю точки на запетые для иксель
                g.write(
                    (";".join(str(x).replace(".", ",") for x in numbers + ["" for x in headers_our]) + "\n")
                )
                continue

            numbers2 = convert_our(our)
            print_aligned(headers_std + headers_our, numbers + numbers2)
            # меняю точки на запетые для иксель
            g.write(
                (";".join(str(x).replace(".", ",") for x in numbers + numbers2) + "\n")
            )

