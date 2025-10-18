#!/bin/bash



if [ -z "$1" ]; then
    echo "Error: Player name is required"
    echo "Usage: $0 <player_name> [ip] [port]"
    echo "Example: $0 alex 192.168.1.100 3000"
    exit 1
fi

if [ ! -d "players_example/$1" ]; then
    echo "Error: Player folder 'players_example/$1' does not exist"
    echo "Available players:"
    ls -1 players_example/ 2>/dev/null | sed 's/^/  - /'
    exit 1
fi

NAME=$1
IP="${2:-0.0.0.0}"
PORT="${3:-2898}"





docker image rm client:$NAME 2>/dev/null
docker build -f dockerfiles/client.dockerfile -t client:$NAME --build-arg NAME=$NAME .




xhost +local:docker

docker run --rm \
  --network=host \
  -e DISPLAY=$DISPLAY \
  -v /tmp/.X11-unix:/tmp/.X11-unix \
  -e NAME="$NAME" \
  -e IP="$IP" \
  -e PORT="$PORT" \
  --device=/dev/dri:/dev/dri \
  --group-add video \
  client:$NAME

xhost -local:docker 2>/dev/null 1>/dev/null
docker image rm client:$NAME 2>/dev/null 1>/dev/null