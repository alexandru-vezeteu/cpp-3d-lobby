FROM debian:bookworm-slim


RUN apt-get update && apt-get install -y --no-install-recommends \
    freeglut3-dev \
    libgl1-mesa-glx \
    libx11-6 \
    libxext6 \
    libxi6 \
    libxmu6 \
    libglew-dev \
    libassimp-dev \
    libglm-dev \
    && rm -rf /var/lib/apt/lists/*







RUN useradd -ms /bin/bash devuser
RUN chown -R devuser:devuser /home/devuser

ARG NAME
ENV IP="0.0.0.0"
ENV PORT="2898"
ENV N="$NAME"

WORKDIR /home/devuser
COPY --chown=devuser:devuser $NAME /home/devuser/$NAME
COPY --chown=devuser:devuser Client /home/devuser/Client
USER devuser

CMD "./Client" "$N" "$IP" "$PORT"