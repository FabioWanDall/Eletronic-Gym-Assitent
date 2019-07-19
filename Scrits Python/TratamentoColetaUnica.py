from NormalizacaoDadosLib import NormalizaDados
import os

os.chdir('.\\..\\Coletas\\Voluntaraio01\\0520')
l = NormalizaDados(10, '02.csv', True, True)
l.criateNewCSV()
print(l.getRegistros())
print(l.getPicos(), l.getVales())
l.criateNewCSV()
l.plotAll()