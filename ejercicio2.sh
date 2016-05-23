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

if [ ! -f $2 ]; then
    printf "%s not found\n" $2
    exit 1
fi

if(( $# == 3 )); then
    my_home=$3
else
    my_home='/ens/home01/d/damian.salvia'
fi;

# ----------------------
#   Otorgar permisos   
# ----------------------
setfacl -m user:algevo00:x $my_home
setfacl -R -m user:algevo00:rwx $my_home/AE_2016
setfacl -m user:nicolas.mechulam:x $my_home
setfacl -R -m user:nicolas.mechulam:rwx $my_home/AE_2016
echo "Permisos otorgados"

# ----------------------
#      Generar bin  
# ----------------------
current=$PWD
cd $my_home/AE_2016
make all
cd $current

# ----------------------
#    Configuracion     
# ----------------------
rm -f $my_home/AE_2016/parametros.in
cant_ciudades=$(sed -n '$=' $current/$1)
echo $cant_ciudades >> $my_home/AE_2016/parametros.in
echo $1 >> $my_home/AE_2016/parametros.in
echo $2 >> $my_home/AE_2016/parametros.in
printf "Parametros:\n\tCiudades: %s\n\tPath matriz: %s\n\tPath temporadas: %s\n" $cant_ciudades $1 $2

# ----------------------
#      Parametros      
# ----------------------
path_cfg=$my_home/AE_2016/"rep/AE-pr/prAE.cfg"
path_params=$my_home/AE_2016/"parametros.in"
path_sol=$my_home/AE_2016/"solucion.out"

# ----------------------
#      Invocacion      
# ----------------------
$my_home/AE_2016/rep/AE-pr/MainSeq $path_cfg $path_params $path_sol   
