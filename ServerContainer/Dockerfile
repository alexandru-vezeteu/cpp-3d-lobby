FROM debian:bookworm-slim


RUN useradd -ms /bin/bash devuser
RUN chown -R devuser:devuser /home/devuser


WORKDIR /home/devuser

COPY --chown=devuser:devuser Server .


ENV IP="0.0.0.0"
ENV PORT="2898"

USER devuser
CMD ./Server "$IP" "$PORT"