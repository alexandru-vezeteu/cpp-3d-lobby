IP="${2:-0.0.0.0}"
PORT="${3:-2898}"
xhost +local:docker
docker run -it --rm \
  --network=host \
  -e DISPLAY=$DISPLAY \
  -v /tmp/.X11-unix:/tmp/.X11-unix \
  -e IP="$IP" \
  -e PORT="$PORT" \
  client:$1