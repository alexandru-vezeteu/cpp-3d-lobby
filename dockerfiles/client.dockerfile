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
RUN cmake -B build && cmake --build build --target Client

FROM debian:bookworm-slim

RUN apt-get update && apt-get install -y --no-install-recommends \
    freeglut3-dev \
    libglew-dev \
    libassimp-dev \
    libgl1 \
    libx11-6 \
    libxext6 \
    libxi6 \
    libxmu6 \
    && rm -rf /var/lib/apt/lists/*

RUN useradd -ms /bin/bash devuser

ARG NAME

WORKDIR /home/devuser
COPY --chown=devuser:devuser players_example/${NAME}/ /home/devuser/${NAME}/
COPY --from=builder --chown=devuser:devuser /app/build/Client /home/devuser/Client

USER devuser

CMD ["sh", "-c", "./Client \"$NAME\" \"$IP\" \"$PORT\""]