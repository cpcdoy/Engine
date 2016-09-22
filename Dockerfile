FROM ubuntu:16.04

MAINTAINER cpcdoy@gmail.com

RUN apt-get update && apt-get install -y \
    build-essential \
    libglm-dev \
    libglfw3-dev \
    libglew-dev \
    libsoil-dev \
    bison \
    flex \
    git \
    cmake

RUN mkdir /Engine
WORKDIR /Engine

ADD . /Engine/

RUN cmake .
RUN make
