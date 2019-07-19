# Este script abre todos os arquivos ja normalizados
# e chama as funções para extrair os dados dos arquivos
# para armazenalos em uma tabela sumarizada
# Tambem solicita a informação do grupo a qual a coleta pertence 
from ExtracaoDadosLib import DadosTempo as extracaoDadosTempo
import numpy as np
import pandas as pd
import os

os.chdir('.\\..\\Normalizados')

columns=[ "Voluntario"
            ,"Grupo"
            ,"SessaoDT"
            ,"Serie"
            ,"NumeroRepetições"
            ,"MedIntervaloDescida"
            ,"MedIntervaloSubida"
            ,"MedIntervaloExtremos"
            ,"VarianciaIntervaloDescida"
            ,"DesvioPadraoIntervaloDescida"]
registros = pd.DataFrame(columns=columns)

#Abrir hierarquia de pastas para acessar os arquivos
for dirName in np.array(os.listdir()): #nomeVoluntario
    if not os.path.isdir(dirName):
        continue 
    os.chdir(dirName)
    grupo = input('O '+dirName+' pertence a qual grupo? (R/L): ')
    for dirDate in np.array(os.listdir()): #dtColeta
        if not os.path.isdir(dirDate):
            continue
        os.chdir(dirDate)
        for file in np.array(os.listdir()):
            print(dirName, dirDate, file)
            registro = np.array([dirName, grupo, dirDate, file])

            edt = extracaoDadosTempo(file)
            info = edt.encontraInfo()
            registro = np.append(registro, info)
            df = pd.DataFrame(data=[registro], columns=columns)
            registros = registros.append(df, ignore_index=True)
        os.chdir('..')
    os.chdir('..')
registros.to_csv('TabelaDeResultadosAnalizeTempoRepeticoes.csv', sep=';', encoding='utf-8', index=False, header=True)
