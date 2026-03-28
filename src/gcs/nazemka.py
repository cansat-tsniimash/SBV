import serial
import madgic

port = serial.Serial("COM4", baudrate=115200)
port.timeout = 1
p = madgic.Parser()

with open("telem.bin", mode="wb") as stream, open("chtoto.csv", mode="w") as g :
    while True:
        data = port.read(4096)
        stream.write(data)
        packets = p.parse(data)
        for packet in packets:
            numbers = madgic.convert()
            g.write(",".join())
            print(numbers)
