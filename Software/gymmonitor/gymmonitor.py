import serial
from pynput import keyboard
import os

Name = input("Nome: ")
Serie = input("Série: ")
print(Name, Serie)

mm = 5 #minimumMovement
position=newPosition=coutRepetition= 0
running=clockwise = True

def on_press(key):
  global running
  print(key)
  if key == keyboard.Key.esc:
    running = False

listener = keyboard.Listener(on_press=on_press)
listener.start()

os.chdir('C:/Users/FabioWanDall/OneDrive - TIGRE S.A/Coleta de dados EGA')
outfile = open(Name+'/'+Serie+'.csv', 'wb')
bufData = b''

with serial.Serial('COM5', 115200, timeout=1) as ser:
  while running:
    data = ser.read()
    if (data != b'\n'):
      bufData += data
    else:
      outfile.write(bufData)
      dataDecoded = bufData.decode()
      print(dataDecoded)
      newPosition = int(dataDecoded.split(",")[0])
      if (position > newPosition) and clockwise:
        clockwise = False
        coutRepetition += 1
        print(coutRepetition)
      elif (position < newPosition):
        clockwise = True
      position = newPosition
      bufData = b''

outfile.close()
listener.stop()

import csv
import matplotlib.pyplot as plt

fil = open(Name+'/'+Serie+'.csv', 'r')
csv_reader = csv.reader(fil, delimiter=',')
x = list()
y = list()
time = 0
for row in csv_reader:
  y.append(int(row[0]))
  time += int(row[1])/1000.0
  x.append(time)
  
plt.plot(x, y)
plt.show()


