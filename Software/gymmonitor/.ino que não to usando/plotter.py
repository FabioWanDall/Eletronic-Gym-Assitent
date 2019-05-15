import csv
import matplotlib.pyplot as plt
import os

Name = input("Nome: ")
Serie = input("Série: ")
print(Name, Serie)

os.chdir('C:/Users/FabioWanDall/OneDrive - TIGRE S.A/Coleta de dados EGA')
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


