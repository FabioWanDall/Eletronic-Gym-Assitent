from NormalizacaoDadosLib import NormalizaDados
import numpy as np
import pandas as pd
import os

#ISSUE#2os.chdir(diretorio indicado externamente)

os.chdir('.\\..\\Coletas')
n1c = os.getcwd()
os.chdir('..')
os.mkdir('Normalizados')
n1n = os.getcwd()+'\\Normalizados'
n0 = os.getcwd()
os.chdir(n1c)
#Abrir hierarquia de pastas para acessar os arquivos
for dirName in np.array(os.listdir()): #nomeVoluntario
    if not os.path.isdir(dirName):
        continue 
    os.chdir(dirName) #entra na pasta
    n2c = os.getcwd() #grava o caminho
    os.chdir(n1n)    #vai pra pasta cNormalizadas
    os.mkdir(dirName) #cria uma pasta para o voluntario
    n2n = os.getcwd()+'\\'+dirName #grava a pasta cn do voluntario  
    os.chdir(n2c) #volta para a pasta de coleta
    for dirDate in np.array(os.listdir()): #dtColeta
        if not os.path.isdir(dirDate):
            continue
        os.chdir(dirDate)
        n3c = os.getcwd() #grava o caminho coletado sesão
        os.chdir(n2n)    #vai pra pasta Normalizadas voluntario
        os.mkdir(dirDate) #cria uma pasta para a sesão
        n3n = os.getcwd()+'\\'+dirDate #grava a pasta n da sesão  
        os.chdir(n3c) #volta para a pasta de sesão

        for file in np.array(os.listdir()):
            print(dirName, dirDate, file)
            l = NormalizaDados(15, file, plotOriginal=True, plotInspecionar=True)
            print(l.getPicos(), l.getVales())
            l.plotAll()
            deuBoa = input('Deu boa? ')
            if deuBoa != 'n' and  deuBoa != 'N':#se normalização bem sucedida
                os.chdir(n3n)#vai para n3normalizado
                l.criateNewCSV() #cria um arquivo com o DF normalizado
                os.chdir(n3c) #volta para n3coletado
            else:
                print('!!!!!!!!!!!!Corrigir....................................')#log para normalizar depois
        os.chdir('..')
    os.chdir('..')
        
