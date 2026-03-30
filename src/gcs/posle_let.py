import madgic # вызываем фаил с нашим основным кодом

p = madgic.Parser()

with open("telem.bin", mode="rb") as stream: # читаем что записали
    while True:
        data = stream.read(4096)
        packets = p.parse(data)
        for packet in packets:
            print(packet) #выводим
