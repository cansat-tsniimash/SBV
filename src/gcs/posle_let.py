import madgic

p = madgic.Parser()

with open("telem.bin", mode="rb") as stream:
    while True:
        data = stream.read(4096)
        packets = p.parse(data)
        for packet in packets:
            print(packet)
