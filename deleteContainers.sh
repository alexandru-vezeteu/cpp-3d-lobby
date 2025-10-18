#!/bin/bash

echo "Finding all client:* images..."

# Get all client images
CLIENT_IMAGES=$(docker images --format "{{.Repository}}:{{.Tag}}" | grep "^client:")

if [ -z "$CLIENT_IMAGES" ]; then
    echo "No client:* images found"
    exit 0
fi

echo "Found the following images:"
echo "$CLIENT_IMAGES"
echo ""

read -p "Do you want to delete these images? (y/N): " -n 1 -r
echo

if [[ $REPLY =~ ^[Yy]$ ]]; then
    echo "Removing images..."
    echo "$CLIENT_IMAGES" | xargs docker image rm
    echo "Done!"
else
    echo "Cancelled"
fi