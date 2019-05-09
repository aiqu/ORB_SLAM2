FROM ubuntu:latest

RUN sed -i -E 's/((archive)|(security)).ubuntu/mirror.kakao/' /etc/apt/sources.list \
      && apt update && apt install -y git \
      && cd $HOME \
      && git clone https://github.com/aiqu/ORB_SLAM2 \
      && cd ORB_SLAM2 \
      && ./install_libs.sh \
      && ./build.sh \
      && rm -rf /var/lib/apt/lists

WORKDIR /root/ORB_SLAM2
