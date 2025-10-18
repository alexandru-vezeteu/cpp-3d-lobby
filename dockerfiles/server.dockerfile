FROM debian:bookworm-slim AS builder

RUN apt-get update && apt-get install -y --no-install-recommends \
    freeglut3-dev \
    libgl1-mesa-dev \
    libx11-dev \
    libxext-dev \
    libxi-dev \
    libxmu-dev \
    libglew-dev \
    libassimp-dev \
    libglm-dev \
    cmake \
    g++ \
    make \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app
COPY src /app
RUN cmake -B build -DBUILD_CLIENT=OFF && cmake --build build --target Server
FROM debian:bookworm-slim


RUN useradd -ms /bin/bash devuser
RUN chown -R devuser:devuser /home/devuser



WORKDIR /home/devuser

COPY --from=builder --chown=devuser:devuser /app/build/Server /home/devuser/Server


ENV IP="0.0.0.0"
ENV PORT="2898"

USER devuser
CMD ./Server "$IP" "$PORT"