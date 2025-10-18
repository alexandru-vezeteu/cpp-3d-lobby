#!/bin/bash


docker image rm server 2> /dev/null
docker build -f dockerfiles/server.dockerfile -t server .

IP="${1:-0.0.0.0}"
PORT="${2:-2898}"

docker run -it --rm \
  --network=host \
  -e IP="$IP" \
  -e PORT="$PORT" \
  --name 3d_lobby_server \
  server

docker image rm server 2>/dev/null 1>/dev/null
