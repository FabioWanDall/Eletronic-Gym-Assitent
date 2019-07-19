import csv
import matplotlib.pyplot as plt
import os

Name = input("Nome: ")
Secao = input("Seção: ")
Serie = input("Série: ")
print(Name, Secao, Serie)

os.chdir('.\\..\\Coletas\\)
fil = open(Name+'\\'+Secao+'\\'+Serie+'.csv', 'r')

csv_reader = csv.reader(fil, delimiter=',')
x = list([])
y = list([])
yd = list([])
time = 0
last = 0
lastTime = 0

for row in csv_reader:
  time += int(row[1])/1000.0
  dt = (int(row[0]) - last)/(time - lastTime) if time!=0 else 0
  
  y.append( int(row[0]) )
  yd.append( dt )
  x.append(time)
  last = int(row[0])
  lastTime = time


fig, ax = plt.subplots(2,1, sharex='col')
ax[0].plot(x, y)
ax[1].plot(x, yd)
plt.show()


