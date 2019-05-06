import serial
from pynput import keyboard

running = True

def on_press(key):
  global running
  print(key)
  if key == keyboard.Key.esc:
    running = False

listener = keyboard.Listener(on_press=on_press)
listener.start()

outfile = open('test.csv', 'wb')

with serial.Serial('COM5', 115200, timeout=1) as ser:
  while running:
    data = ser.read()
    outfile.write(data)
    print(data)

outfile.close()
listener.stop()

import csv
import matplotlib.pyplot as plt

fil = open('test.csv', 'r')
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


