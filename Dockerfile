FROM debian:bookworm-slim
LABEL Description="Docker container for building DuneII for Linux"

ENV External=/home/eugene/external

# Install dependencies
RUN apt-get update && apt-get install --no-install-recommends -y \
    make \
    xauth \
    gdbserver \
    libopenal-dev \
    xauth \
    bash \
    gdb \
    libgl1-mesa-dev \
    x11-apps \
    mesa-utils \
    uuid-dev \
    nasm \
    build-essential \
    perl \
    python3 \
    pkg-config \
    libxcb*-dev \
    libfontconfig1-dev \
    libx11-xcb-dev \
    libxrender-dev \
    libxrandr-dev \
    libxcursor-dev \
    libxi-dev \
    libxkbcommon-dev \
    libxkbcommon-x11-dev \
    libxinerama-dev \
    libwayland-dev \
    libicu-dev \
    libxslt-dev \
    libfreetype6-dev \
    libvorbis-dev \
    libssl-dev \
    libcurl4-openssl-dev \
    ca-certificates \
    git \
    wget \
    dos2unix \
    lsb-release \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /home/eugene
ARG CMAKE_VERSION=4.2.0
RUN wget https://github.com/Kitware/CMake/releases/download/v${CMAKE_VERSION}/cmake-${CMAKE_VERSION}.tar.gz && tar xvzf "cmake-${CMAKE_VERSION}.tar.gz" && cd cmake-${CMAKE_VERSION} && \
    chmod +x configure && ./configure && make && make install && cd .. && rm -rf cmake-${CMAKE_VERSION} && rm cmake-${CMAKE_VERSION}.tar.gz

# Project workspace
ARG USER=eugene
ARG UID=1000
ARG GID=1000
RUN useradd -m ${USER} --uid=${UID}
USER ${UID}:${GID}

WORKDIR /home/eugene/project
COPY external ${External}