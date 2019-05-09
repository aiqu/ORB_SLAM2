#!/usr/bin/env bash

set -e

function cleanup {
  xhost -local:root
}
trap 'cleanup' TERM INT HUP EXIT

xhost +local:root

if docker ps -a --format '{{ .Names }}' | grep orbslam -q; then
  docker start -i orbslam
else
  if [ -d /dev/dri ]; then
    HW+=' --device /dev/dri'
  fi
  if [ -d /dev/snd ]; then
    HW+=' --device /dev/snd'
  fi
  if [ -d /dev/video0 ]; then
    HW+=' --device /dev/video0'
  fi
  docker run -ti \
    --name orbslam \
    --net host \
    -e DISPLAY \
    $HW \
    -v /tmp/.X11-unix:/tmp.X11-unix:rw \
    -v /etc/localtime:/etc/localtime:ro \
    -v $(pwd):/data \
    gwangmin/orbslam:latest
fi
