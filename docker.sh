#!/bin/bash

EXTERNAL_SOURCE_DIR=$External
PROJECT_SOURCE_DIR=$PWD

mkdir -p $PROJECT_SOURCE_DIR/external
cp -r $EXTERNAL_SOURCE_DIR/glfw $PROJECT_SOURCE_DIR/external
cp -r $EXTERNAL_SOURCE_DIR/glad $PROJECT_SOURCE_DIR/external
cp -r $EXTERNAL_SOURCE_DIR/cglm $PROJECT_SOURCE_DIR/external
cp -r $EXTERNAL_SOURCE_DIR/rapidxml $PROJECT_SOURCE_DIR/external
cp -r $EXTERNAL_SOURCE_DIR/stb $PROJECT_SOURCE_DIR/external

docker build -t dune .
rm -rf external

# Then in project dir: 
# docker run --rm -it --mount type=bind,src=$(pwd),target=/home/eugene/project dune bash -c "cmake -S. -Bbuild -DCMAKE_BUILD_TYPE=Release && cmake --build build"

