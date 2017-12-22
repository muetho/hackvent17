import os
import qrtools

os.system("curl --output qr.png http://challenges.hackvent.hacking-lab.com:4200")

qr = qrtools.QR()
qr.decode("qr.png")
print(qr.data)
