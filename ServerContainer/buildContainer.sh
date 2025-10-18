docker image rm server
docker build -t server . --build-arg IP=$1 --build-arg PORT=$2
