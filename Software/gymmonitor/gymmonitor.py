import serial
from pynput import keyboard
import os

Name = input("Nome: ")
Serie = input("Série: ")
print(Name, Serie)

minimumMovement = 5
position=newPosition=coutRepetition=positionWhenDirectionChange=moviment= 0
running=clockwise=counted = True

def on_press(key):
  global running
  print(key)
  if key == keyboard.Key.esc:
    running = False

listener = keyboard.Listener(on_press=on_press)
listener.start()

os.chdir('C:/Users/FabioWanDall/OneDrive - TIGRE S.A/dados EGA')
with open(Serie+'.csv', 'wb') as outfile:
  bufData = b''

  with serial.Serial('COM5', 115200, timeout=1) as ser:
    while running:
      data = ser.read()
      if (data != b'\n'):
        bufData += data
      else:
        outfile.write(bufData)
        dataDecoded = bufData.decode()
        #print(dataDecoded)
        newPosition = int(dataDecoded.split(",")[0])
        if (position < newPosition) and not clockwise: # sentido horario pela primeira vez
          clockwise = True
          counted = True
          positionWhenDirectionChange = position
        elif (position > newPosition) and clockwise: # sentido antihorario pela primeira vez
          clockwise=counted = False
          positionWhenDirectionChange = position
        
        moviment = abs(positionWhenDirectionChange - newPosition)

        
        print( position, newPosition, positionWhenDirectionChange, 'Horario' if clockwise else 'Antiorario' , moviment, 'Já Contei' if counted else 'Não Contei', coutRepetition)
        if (moviment >= minimumMovement) and not counted:
          coutRepetition += 1
          counted = True
          print('coutRepetition', coutRepetition)

        bufData = b''
        position = newPosition

listener.stop()

import csv
import matplotlib.pyplot as plt

fil = open(Serie+'.csv', 'r')
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


