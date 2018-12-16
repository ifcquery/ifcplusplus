FROM ubuntu:18.04

RUN apt-get update && \
    apt-get install -y software-properties-common cmake libboost-dev && \
    add-apt-repository ppa:ubuntu-toolchain-r/test && \
    apt-get install -y g++-8 && \
    update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-8 60 --slave /usr/bin/g++ g++ /usr/bin/g++-8 

RUN apt-get install -y \
        cmake                   \
        qt5-qmake               \
        libqt5widgets5          \
        libqt5opengl5-dev       \
        qttools5-dev            \
        qtbase5-dev             \
        libxrandr-dev           \
        libtiff5-dev            \
        libpoppler-glib-dev     \
        librsvg2-dev            \
        libcairo2-dev           \
        libcurl4-gnutls-dev     \
        libgtkglext1-dev        \
        libgdal-dev             \
        libsdl1.2-dev           \
        libgstreamer1.0-dev     \
        libopenjp2-7-dev        \
        libopenscenegraph-dev
