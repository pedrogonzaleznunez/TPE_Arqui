# This script is used to compile the TPE project.
# It is designed to be run in a Docker container.

# Enter docker container name
CONTAINER_NAME="TPE_ARQUI"  # Corregido para que coincida con el nombre real del contenedor

# Iniciamos el contenedor
docker start $CONTAINER_NAME
docker exec -it $CONTAINER_NAME make clean -C /root/Toolchain
docker exec -it $CONTAINER_NAME make clean -C /root/
docker exec -it $CONTAINER_NAME make -C /root/Toolchain
docker exec -it $CONTAINER_NAME make -C /root/

COMPILE_STATUS=$?

# Contar advertencias y errores en la salida de compilación
# Guardar la salida de compilación en un archivo temporal
COMPILE_LOG=$(mktemp)
docker exec -it $CONTAINER_NAME make -C /root/Toolchain > "$COMPILE_LOG" 2>&1
docker exec -it $CONTAINER_NAME make -C /root/ >> "$COMPILE_LOG" 2>&1

# Contar advertencias y errores en la salida de compilación
WARNINGS=$(grep -i "warning:" "$COMPILE_LOG" | wc -l)
ERRORS=$(grep -i "error:" "$COMPILE_LOG" | wc -l)

# Verificamos si la compilación fue exitosa
if [ $COMPILE_STATUS -eq 0 ]; then
    echo "\n#############################################################"
    echo "\033[1;32mOK! - Compiled Succesfully\033[0m"
    echo "#############################################################"

else
    echo "\n#############################################################"
    echo "\033[1;31mERROR\033[0m - La compilación falló con código de salida $COMPILE_STATUS."
    echo "#############################################################"
fi
# Mostrar advertencias y errores

echo "\033[1;33mWARNINGs\033[0m - $WARNINGS "
echo "\033[1;33mERRORs\033[0m - $ERRORS"
echo "#############################################################"

#docker stop $CONTAINER_NAME

exit 0