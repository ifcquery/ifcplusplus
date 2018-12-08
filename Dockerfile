FROM ubuntu:18.04

RUN apt-get update && \
    apt-get install -y software-properties-common cmake libboost1.62-dev && \
    add-apt-repository ppa:ubuntu-toolchain-r/test && \
    apt-get install -y g++-8 && \
    update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-8 60 --slave /usr/bin/g++ g++ /usr/bin/g++-8 
