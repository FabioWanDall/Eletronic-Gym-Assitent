import csv
import matplotlib.pyplot as plt
import time

x = list([0])
y = list([0])

fig = plt.figure()
plt.grid()
plt.ion()
plt.show()

fil = open('1.csv', 'r')
csv_reader = csv.reader(fil, delimiter=',')
acc_time = 0
for row in csv_reader:
  y.append(int(row[0]))
  acc_time += int(row[1])/1000.0
  x.append(acc_time)
  plt.stem(x,y)
  plt.draw()
  plt.pause(0)
  
  
plt.show()


