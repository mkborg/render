FROM debian

RUN \
  apt-get update \
  && \
    DEBCONF_NONINTERACTIVE_SEEN=true \
    DEBIAN_FRONTEND=noninteractive \
    apt-get install -y \
      gcc \
  && true

# pwd == '/'

RUN \
  ls -la \
  && true

RUN \
  ls -la /root/ \
  && true

RUN \
  ls -la /home/ \
  && true

RUN \
  find /root -name consume_memory.c 2>/dev/null \
  || true

RUN \
  find /home -name consume_memory.c 2>/dev/null \
  || true

RUN \
  find / -name consume_memory.c 2>/dev/null \
  || true

RUN \
  gcc \
    -o /consume_memory \
    src/consume_memory.c \
  && true

#COPY /consume_memory consume_memory

# ENTRYPOINT and CMD are two different instructions. If they both
# exist, CMD would be treated as ENTRYPOINT’s parameters.
#
# Default is like the following:
#   ENTRYPOINT ["/bin/sh", "-c"]
#   CMD []

ENTRYPOINT ["/consume_memory"]
CMD []
