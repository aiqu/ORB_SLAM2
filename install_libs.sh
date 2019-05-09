#!/usr/bin/env bash

set -e

if [ $UID -ne 0 ]; then
  S='sudo'
fi

if ! cat /etc/os-release | grep -q Ubuntu; then
  echo 'only support Ubuntu distro'
  exit 1
fi

$S sed -i -E 's/((archive)|(security)).ubuntu/mirror.kakao/' /etc/apt/sources.list
$S apt update
DEBIAN_FRONTEND=noninteractive $S apt install -y libpython-dev libopencv-dev g++-7 libeigen3-dev libglew-dev cmake git

if [ ! -r /usr/local/lib/libpangolin.so ] || [ ! -r /usr/local/include/pangolin/pangolin.h ]; then
  WORKDIR=$(mktemp -d)
  pushd $WORKDIR
  git clone --depth=1 https://github.com/stevenlovegrove/Pangolin
  cd Pangolin
  mkdir build; cd build
  cmake -DBUILD_TESTS=OFF -DBUILD_TOOLS=OFF -DBUILD_EXAMPLES=OFF ..; make -j$(nproc) install
  popd
  rm -rf $WORKDIR
fi
