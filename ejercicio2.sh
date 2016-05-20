#!/bin/bash

# ----------------------
# Verificar parametros 
# ----------------------
if (( $# < 2 )); then
	echo './ejercicio.sh <path_matriz> <path_temporadas>'
	exit 1
fi;

if [ ! -f $1 ]; then
    printf "%s not found\n" $1
    exit 1
fi;

if [ ! -f $2]; then
    printf "%s not found\n" $2
    exit 1
fi

# ----------------------
#   Otorgar permisos   
# ----------------------
setfacl -m user:algevo00:x HOME
setfacl -R -m user:algevo00:rwx HOME/AE_2016
echo "Permisos otorgados"

# ----------------------
#      Generar bin  
# ----------------------
make all

# ----------------------
#    Configuracion     
# ----------------------
rm parametros.in
cant_ciudades=$(sed -n $= $1)
echo $cant_ciudades >> parametros.in
echo $1 >> parametros.in
echo $2 >> parametros.in
printf "Parametros:\n\tCiudades: %s\n\tPath matriz: %s\n\tPath temporadas: %s\n" $cant_ciudades $1 $2

# ----------------------
#      Parametros      
# ----------------------
path_cfg="rep/AE-pr/prAE.cfg"
path_params="parametros.in"
path_sol="solucion.out"

# ----------------------
#      Invocacion      
# ----------------------
./rep/AE-pr/MainSeq $path_cfg $path_params $path_sol   