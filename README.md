Eletronic Gym Assitent (E-GA)

Conjunto de software e hardware para a contrução de um monitorador de equipamentos de musculação baseados em polia.

03/03/2019-22:15
Devido ao novo design que foi pensado para esta aplicação o codigo antigo não faz mais sentido, por isso um novo codigo 
será escrito desde o inicio visando atender os requesitos.
O EGA device deve enviar um registro a cada fim de curso(metade do movimento completo), cada registro deve conter a repetição
da qual ele faz parte(dois registros por repetição), a amplitude do movimento(Ticks do encoder) e o tempo para realização
(obtido pela função milis()). Formato NRepetição;Amplitude;Tempo. 
fabio 