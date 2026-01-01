FROM busybox

COPY consume_memory consume_memory

# ENTRYPOINT and CMD are two different instructions. If they both
# exist, CMD would be treated as ENTRYPOINT’s parameters.
#
# Default is like the following:
#   ENTRYPOINT ["/bin/sh", "-c"]
#   CMD []

ENTRYPOINT ["/consume_memory"]
CMD []
