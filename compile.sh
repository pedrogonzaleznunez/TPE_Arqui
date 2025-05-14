# This script is used to compile the TPE project.
# It is designed to be run in a Docker container.

# Enter docker container name
CONTAINER_NAME="TPE"

sudo docker start $CONTAINER_NAME
sudo docker exec -it $CONTAINER_NAME make clean –C /root/Toolchain
sudo docker exec -it $CONTAINER_NAME make clean –C /root/
sudo docker exec -it $CONTAINER_NAME make –C /root/Toolchain
sudo docker exec -it $CONTAINER_NAME make –C /root/

# este comando va comentado porq tarda mucho
#sudo docker stop $CONTAINER_NAME
