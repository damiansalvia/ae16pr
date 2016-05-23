import os
import sys
import re
import math

if len(sys.argv) < 4:
	 raise Exception('estadisitcas.py <cantidad_de_ejecuciones> <path_matriz> <path_temporadas>')

# Get parametros
cantidad = int(sys.argv[1])
path_matriz = str(sys.argv[2])
path_temporadas = str(sys.argv[3])

# Ruta
ruta = '../'
#ruta = '../rep/AE-pr/'

# Commando
comando = '../ejercicio2 '+path_matriz+' '+path_temporadas
print comando
#comando = "make SEQ "

class Solucion:

	def __init__(self,sol,iteracion,costo,tiempo):
		self.sol = sol
		self.iteracion = iteracion
		self.costo = costo
		self.tiempo = tiempo

	def __str__(self):
		return "%s,%i,%i,%i" % (self.sol, self.iteracion, self.costo, self.tiempo)

def imprimirBarra(idx,cant):
	porcent = int((float(idx)/(cant-1) * 100))
	print '\rProcesando: ' + '[' + '#' * (porcent/2) + '-' * (50 - (porcent/2)) +'] ' + str(porcent) +'%',
	sys.stdout.flush()


soluciones = []
archSol = open('solucion.csv', 'w')


for idx in range(cantidad):
	
	print comando
	os.system(comando + ' > temp')

	temp = open('temp')
	lineas = temp.read()

	match = re.search('Solution:\s*(.*)',lineas)
	sol = match.group(1)

	match = re.search('Iteration:\s*(\d+)',lineas)
	itera = match.group(1)

	match = re.search('Fitness:\s*(\d+)',lineas)
	fit = match.group(1)
	
	time = '000'
	time = str(int(re.sub('e\+(\d+)','', time)) * 10^int(m.group(1))) if m else time

	soluciones.append(Solucion(sol,int(itera),int(fit),int(time)))

	temp.close()
	imprimirBarra(idx,cantidad)

os.system('rm temp')
archSol.write('Estadisticas:\n\n')

avgI = reduce(lambda x, y: x + y, [float(sol.iteracion) for sol in soluciones]) / len(soluciones)
avgC = reduce(lambda x, y: x + y, [float(sol.costo) for sol in soluciones]) / len(soluciones)
avgT = reduce(lambda x, y: x + y, [float(sol.tiempo) for sol in soluciones]) / len(soluciones)

var = sum([math.pow(sol.costo - avgC,2) for sol in soluciones])

archSol.write('Media:,'+str(avgC)+'\n')
archSol.write('Varianza:,'+str(var)+'\n')
archSol.write('Des. Estandart:,'+str(math.sqrt(var))+'\n\n')

archSol.write('Promedio:\n')
archSol.write('Iteraciones,Coste,Tiempo\n')
archSol.write(str(avgI) + ','+ str(avgC) + ','+ str(avgT) + ',\n\n')

archSol.write('Mejor:\n')
archSol.write('Solucion,Iteraciones,Coste,Tiempo\n')
s = min(soluciones, key=lambda x : x.costo)
archSol.write(str(s) + '\n\n')

archSol.write('Peor:\n')
archSol.write('Solucion,Iteraciones,Coste,Tiempo\n')
s = max(soluciones, key=lambda x : x.costo)
archSol.write(str(s) + '\n\n')

archSol.write('Solucion, Iteracion, Fitness, Tiempo\n')
archSol.writelines([str(sol) + '\n' for sol in soluciones])
