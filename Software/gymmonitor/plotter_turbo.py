import csv
import matplotlib.pyplot as plt
import os

Name = input("Nome: ")
Serie = input("Série: ")
print(Name, Serie)

os.chdir('C:/Users/FabioWanDall/OneDrive - TIGRE S.A/Coleta de dados EGA')
fil = open(Name+'/'+Serie+'.csv', 'r')

csv_reader = csv.reader(fil, delimiter=',')
x = list([])
x2 = list([])
y = list([])
yd = list([])
y2 = list([])
time = 0
last = 0
last2 = 0
lastTime = 0

x2.append([])
y2.append([])
for row in csv_reader:
  time += int(row[1])/1000.0
  dt = (int(row[0]) - last)/(time - lastTime) if time!=0 else 0
  dt2 = (dt - last2)/(time - lastTime) if time!=0 else 0
  if (last2 < 0 and dt2 > 0) or (last2 >0 and dt2 < 0):
    x2.append([])
    y2.append([])
  y.append( int(row[0]) )
  yd.append( dt )
  y2[-1].append( dt2 )
  x.append(time)
  x2[-1].append(time)
  last = int(row[0])
  last2 = int(dt)
  lastTime = time


fig, ax = plt.subplots(3,1, sharex='col')
ax[0].plot(x, y)
ax[1].plot(x, yd)
for x_l, y_l in zip(x2,y2):
  ax[2].plot(x_l, y_l)
plt.show()


