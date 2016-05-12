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

4) Compilar libreria
> make all

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

