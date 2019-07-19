# A ideia aqui é ler um arquivo de coleta ja normalizado
# e extrair dele o tempo medio entre pico-vale e vale-pico.
# conta repeticao e calcula a media de tempo entre picos e vales, variancia e DP
import numpy as np
import pandas as pd

class DadosTempo():
    def __init__(self, fileName):
        self.fileName = fileName
        # self.filesPath = os.getcwd()
        self.dfRegistros = pd.read_csv(self.fileName, names= ['Posicao', 'Intervalo', 'Tempo', 'pv'], sep=';')
    
    def encontraInfo(self):
        """Retorna um pd.serie com medPV, medVP, medPVP e nRepetições"""
        picos = self.dfRegistros.loc[self.dfRegistros['pv'] == 1]
        vales = self.dfRegistros.loc[self.dfRegistros['pv'] == -1]
        
        somaTempoValePico = somaTempoPicoVale = 0
        intervalos = list()
        nRepeticoes = picos['pv'].size

        for i in range(nRepeticoes):
            tempoValePico = picos.iloc[i]['Tempo'] - vales.iloc[i]['Tempo']
            somaTempoValePico = somaTempoValePico + tempoValePico   #soma tempos de descida
            tempoPicoVale = vales.iloc[i+1]['Tempo'] - picos.iloc[i]['Tempo']   
            somaTempoPicoVale = somaTempoPicoVale + tempoPicoVale   #soma tempos de subida

            intervalos.append(tempoValePico)
            intervalos.append(tempoPicoVale)

        medVP = somaTempoValePico/nRepeticoes #media (vp) descida
        medPV = somaTempoPicoVale/nRepeticoes #media (pv) subida
        medPVP = (medPV + medVP) / 2    #media geral entre os pontos

        auxVariancia = 0
        for x in intervalos:
            auxVariancia = auxVariancia + ((x - medPVP) ** 2)
        variancia = auxVariancia / len(intervalos)
        desvioPadrao = variancia ** (1/2)
        
        return np.array([int(nRepeticoes), int(medVP), int(medPV), int(medPVP), int(variancia), int(desvioPadrao)])
        