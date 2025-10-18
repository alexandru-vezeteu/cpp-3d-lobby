IP="${1:-0.0.0.0}"
PORT="${2:-2898}"

docker run -it --rm \
  --network=host \
  -e IP="$IP" \
  -e PORT="$PORT" \
  server