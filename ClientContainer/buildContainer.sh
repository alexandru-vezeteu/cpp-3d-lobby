docker image rm client:$1
docker build -t client:$1 --build-arg NAME=$1 .