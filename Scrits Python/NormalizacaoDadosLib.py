import os
import pandas as pd
import matplotlib.pyplot as plt

class NormalizaDados():
    def __init__(self, minimumMovement, fileName, plotOriginal=False, plotInspecionar=False):
        self.minimumMovement = minimumMovement
        self.fileName = fileName
        # os.chdir('C:\\Users\\FabioWanDall\\OneDrive - TIGRE S.A\\Dados EGA\\Coletas\\Bruno\\0517')
        self.filesPath = os.getcwd()
        self.dfRegistros = pd.read_csv(self.fileName, names= ['Posicao', 'Intervalo', 'Tempo'])
        self.plotInspecionar = plotInspecionar
        if plotOriginal:
            self.plotRegistros()
        self.picos = []
        self.vales = []
        self.__encontraPicosVales()
        self.__filtraRepeticoes()
        self.__addTempo_pv()

    def __encontraPicosVales(self):
        position=newPosition=indexWhenDirectionChangeToClockWise = 0
        clockwise = True
        
        for i, row in self.dfRegistros.iterrows():
            newPosition = int(row['Posicao'])
            if (position < newPosition) and not clockwise: # sentido horario pela primeira vez
                clockwise = True
                indexWhenDirectionChangeToClockWise = i
            elif (position > newPosition) and clockwise: # sentido antihorario pela primeira vez
                clockwise = False
        
            i-=1
            if i+self.minimumMovement < self.dfRegistros['Posicao'].index.max():
                if clockwise: #crescente (é um vale)
                    if i >= self.minimumMovement:
                        if self.dfRegistros.iloc[i]['Posicao'] + self.minimumMovement == self.dfRegistros.iloc[i-self.minimumMovement]['Posicao']: #Posicao tras/cima 
                            if self.dfRegistros.iloc[i]['Posicao'] + self.minimumMovement == self.dfRegistros.iloc[i+self.minimumMovement]['Posicao']: #movimento frente/cima
                                self.vales.append(i)
                else: #decrescente (é um pico)
                    if self.dfRegistros.iloc[i]['Posicao'] - self.minimumMovement == self.dfRegistros.iloc[i+self.minimumMovement]['Posicao']: #Posicao frente/baixo e nao he o fim
                        if self.dfRegistros.iloc[i]['Posicao'] - self.minimumMovement == self.dfRegistros.iloc[i-self.minimumMovement]['Posicao']: #movimento tras/baixo
                            if indexWhenDirectionChangeToClockWise > 0 and len(self.vales) == 0:
                                self.vales.append(indexWhenDirectionChangeToClockWise)
                            self.picos.append(i)
            else:
                self.dfRegistros.at[i, 'Posicao'] = -1
            i+=1
            position = newPosition

        if self.vales.count(indexWhenDirectionChangeToClockWise-1) == 0:
            self.vales.append(indexWhenDirectionChangeToClockWise-1)
        # return [self.picos, self.vales]
        #Aqui ja tem picos e valor

    def __filtraRepeticoes(self):
        #Descobre menor vale
        menorVale = 999
        for i in self.vales:
            if menorVale > self.dfRegistros.iloc[i]['Posicao']:
                menorVale = self.dfRegistros.iloc[i]['Posicao']

        #Subtrai menor vale de todo mundo
        self.dfRegistros['Posicao'] = self.dfRegistros['Posicao'] - menorVale

        #Corta negativos e antes do primeiro vale
        valoresNegativos = []
        if self.plotInspecionar:
            # print('Normalizado com menor vale')
            self.plotAll()
        for i, row in self.dfRegistros.iterrows():
            if row['Posicao'] < 0:
                valoresNegativos.append(i)
        self.dfRegistros = self.dfRegistros.drop(valoresNegativos)
        self.__ajutaValesInicioFim()
        valoresIntesIniDepoisFim = []
        for i, row in self.dfRegistros.iterrows():
            if i < self.vales[0] or i > self.vales[-1]:
                valoresIntesIniDepoisFim.append(i)
        self.dfRegistros = self.dfRegistros.drop(valoresIntesIniDepoisFim)
        self.dfRegistros['Posicao'] = self.dfRegistros['Posicao'].astype(int)

    def __ajutaValesInicioFim(self):
        if self.picos[-1] > self.vales[-1]:
            self.vales.append(self.dfRegistros['Posicao'].index.max())
        if self.picos[0] < self.vales[0]:
            self.vales.insert(0, self.dfRegistros['Posicao'].idxmin())

    def getRegistros(self):
        return self.dfRegistros

    def getPicos(self):
        return self.picos

    def getVales(self):
        return self.vales
    
    def plotAll(self):
        #plota coleta, picos e vales
        x = list()
        y = list()
        y2 = list()
        x2 = list()
        y3 = list()
        x3 = list()
        time = 0
        for i, row in self.dfRegistros.iterrows():
            y.append(int(row['Posicao']))
            time += int(row['Intervalo'])/1000.0
            x.append(time)
            if i in self.picos:
                y2.append(int(row['Posicao']))
                x2.append(time)
            if i in self.vales:
                y3.append(int(row['Posicao']))
                x3.append(time)
        plt.plot(x3, y3)
        plt.plot(x2, y2)
        plt.plot(x, y)
        plt.show()

    def plotRegistros(self):
        #plota coleta, picos e vales
        x = list()
        y = list()
        time = 0
        for i, row in self.dfRegistros.iterrows():
            y.append(int(row['Posicao']))
            time += int(row['Intervalo'])/1000.0
            x.append(time)
        # plt.stem(x, y)
        plt.plot(x, y)
        plt.show()

    def __addTempo_pv(self):
        tempo = 0
        for i, row in self.dfRegistros.iterrows():
            if self.picos.count(i) > 0:
                self.dfRegistros.at[i, 'pv'] = 1
            elif self.vales.count(i) > 0:
                self.dfRegistros.at[i, 'pv'] = -1
            else:
                self.dfRegistros.at[i, 'pv'] = 0
            self.dfRegistros.at[i, 'Tempo'] = tempo
            tempo += row['Intervalo']
            
        self.dfRegistros['Tempo'] = self.dfRegistros['Tempo'].astype(int)
        self.dfRegistros['pv'] = self.dfRegistros['pv'].astype(int)

    def criateNewCSV(self):
        self.dfRegistros.to_csv('new'+self.fileName, sep=';', encoding='utf-8', index=False, header=False)
        
        #print(os.path.dirname(os.path.abspath(__file__)))#onde o arquivo esta salvo
        #print(os.getcwd())#onde eu estou mechendo
        # os.chdir('..')
        # os.chdir(self.filesPath)