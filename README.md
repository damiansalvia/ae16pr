## Proyecto AE
Proyecto para la asignatura Algoritmos Evolutivos (Facultad de Ingeniería, UdelaR) utilizando MALLBA.

### Errores
#### Parser 
Eclipse puede presentar errores de sintaxis, pero al compilar por terminal no existen.
La solución a este problema se describe a continuación:
1) Ir a "Proyect properties > C/C++ Builds > Settings"
2) Ubicarse en la solapa "Tool Settings" y buscar "GCC C++ Compiler"
3) Debería verse
:Compiler: gcc
:All options: _-std=g++1x_ -I/<path>/src -O3 -g3 -Wall -c -fmessage-length=0
:: Para ver _-std=gnu++1x_ se debe escribir en "Dialect" en el _other dialect flags_ 

#### Makefile
Para compilar con tu propio makefile hay que seguir estos pasos:
:1) Ir a "Proyect properties > C/C++ Builds"
:2) Deshabilitar la opción "Generate Makefiles Automatically" y fijarse que el "Build location" sea el path a donde se encuentra el makefile principal.
:3) Abrir la vista "Make target"
:4) Con la opción "Botón Derecho > Make target" se puede ejecutar el makefile

