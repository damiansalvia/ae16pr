#Proyecto AE
Proyecto para la asignatura Algoritmos Evolutivos (Facultad de Ingeniería, UdelaR) utilizando MALLBA.

---

##Introducción

Mallba es una biblioteca de esqueletos para optimización combinatoria que deben instanciarse con las características de un problema y su solución.<br>    
<br>
Antes de comenzar cabe destacar que clasificamos a cada una de las clases en una de las siguientes categorías: 
- **Required** (nombre.req.cc): Clase cuyos atributos y métodos definen las características del problema concreto (i.e. Problem, Solution, etc).
- **Provider** (nombre.pro.cc): Clase cuyos atributos y métodos son independiente del problema concreto a resolver en el cual se instanciará, por lo tanto dicha clase será provista por Mallba (i.e. Solver , SetupParams, etc).
- **Interna**: Clase que no tiene significado en si, necesarias para la implementación interna de Mallba pero que no significan ningún concepto desde el punto de vista del algoritmo genético (i.e. NetStream, StateCenter etc).

![arquitectura](uml-comun.gif)

---

##Descripción del problema
Un espia debe realizar una misión secreta visitando cierta cantidad de ciudades en determinada cantidad de 
días partiendo siempre de la misma ciudad. No debe visitar la misma ciudad dos veces y cada ciudad la visitará 
exatamente cinco días. 

Los costos del viaje entre ciudades están previamente estipulados para temporada baja, ocacionalmente de forma asimétrica (el costo de ida y de vuelta pude variar o puede no existir un viaje bidireccional - valor *-1* en la matriz de costo) y está sujeto a un incremento procentual por temporadas media y alta (10% y 30% respectivamente). Dado que la moneda corriente no acepta fraciones de unidad, el costo resultante se debe redondear al entero más cercano.

El problema consiste en encontrar la ruta más económica para que el espía pueda realizar su misión cumpliendo con todas las restricciones plateadas.

---

##Descripción de la solución
Se plantea la necesidad de contar con un camino que describa la ruta que hará el espía por las ciudades economizando su costo monetario, considerando que el mismo está sujeto a las zafras por temporada y por consiguiente a las estadías en cada ciudad.

De esta forma, la solución será una secuencia de identificadores de ciudades de enteros cuyo índice indicará qué ciudad visitará el espía en determinada ocasión (p.e. para el caso de cinco ciudades, el vector *<4,2,5,3,1>* indica que primero visita la ciudad 4, luego la 2, y así sucesivamente).

La factibilidad de la solución estará dada por ciertas características:
* La primer ciudad deberá ser el origen (identificador 0)
* El camino deberá ser una permitación de identificadores de las ciudades entre 0...cantidad-1
* Para cada par de ciudades <i,j> adyacentes en la ruta el costo(i,j) != -1 

De esta manera, el problema se puede formalizar como:<br>

> Conociendo

>>![eq_ciudades](img/eq_ciudades.png)

>>![eq_costos](img/eq_costos.png)

>>![eq_temporadas](img/eq_temporadas.png)

>Se busca

>>![eq_camino](img/eq_camino.png)

---

##Instalación

1) Instalar MPICH

>sudo apt-get install mpich

2) Clonar repositorio

>git clone https://github.com/damiansalvia/ae16pr.git

3) Modificar el archivo *environment*

>\# Malva Configuration<br>
>Malva\_DIR=<path\_to\_malva><br>
>\# MPICH library<br>
>MPI_BIN=/usr/bin<br>


---

#Recursos
1) Se proveen de dos scripts para *Python 2.7*:<br>
`generación.py`: Calcula la matriz de costo y temporadas<br>
`validación.py`: Evalúa la factibilidad de la solución según la matriz de costos y temporadas<br>
<br>
2) Se provee un script *bash* para la ejecución completa del algoritmo según los parámetros indicados<br>

---

##Parametros *.cfg
* **Operadores de seleccion**<br>
 	0:Random <br> 
	1:Torneo<br> 
	2:Ruleta / Proporcional<br> 
	3:Rank<br>
	4:Best <br>
	5:Worst <br><br>
* **Operadores de cruzamiento & mutación**<br>
	0: PMX v1<br>
	1: Mutacion<br>
	2: PMX v2<br>
	3: OX<br>
	4: CX<br>
	5: ER<br>

---

##Ejecución
1) Ejecutar script

> ./ejercicio2.sh path\_matriz path\_temporadas

2) Se generan dos archivos en "/"

- *parametros.in* con la entrada 

- *solucion.out* con la salida 

**Ejemplo**: Para ejercicio1 se debe invocar así:

> ./ejercicio2.sh ins/ej1/ej1\_matriz ins/ej1/ej1\_temporadas

Alternativamente

> cd rep/AE-pr

> make SEQ

---
## Usando Eclipse CDT - Problemas comunes

**Parser**<br> 
Eclipse puede presentar errores de sintaxis, pero al compilar por terminal no existen.<br>
La solución a este problema se describe a continuación:<br>
1) Ir a "Proyect properties > C/C++ Builds > Settings"<br>
2) Ubicarse en la solapa "Tool Settings" y buscar "GCC C++ Compiler"<br>
3) Debería versee<br>
:Compiler: gcc<br>
:All options: _-std=g++1x_ -I/<path>/src -O3 -g3 -Wall -c -fmessage-length=0<br>
 Para ver _-std=gnu++1x_ se debe escribir en "Dialect" en el _other dialect flags_ <br><br>

**Makefile**<br>
Para compilar con tu propio makefile hay que seguir estos pasos:<br>
1) Ir a "Proyect properties > C/C++ Builds"<br>
2) Deshabilitar la opción "Generate Makefiles Automatically" y fijarse que el "Build location" sea el path a donde se encuentra el makefile principal.<br>
3) Abrir la vista "Make target"<br>
4) Con la opción "Botón Derecho > Make target" se puede ejecutar el makefile

