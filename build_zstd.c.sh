#!/bin/bash

if [ ! -d "./build_zstd_c" ]; then
    mkdir -p ./build_zstd_c
fi

git clone https://github.com/facebook/zstd.git ./build_zstd_c

if [ $? -ne 0 ]; then
    echo "Git clone failed. Exiting. . ."
    exit 1
fi

if [ ! -d "./lib/zstd" ]; then
    mkdir -p ./lib/zstd
fi

python3 ./build_zstd_c/build/single_file_libs/combine.py \
    -r ./build_zstd_c/lib \
    -x legacy/zstd_legacy.h \
    -o ./lib/zstd/zstd.c \
    ./build_zstd_c/build/single_file_libs/zstd-in.c

if [ $? -ne 0 ]; then
    echo "Python script failed. Exiting. . ."
    exit 1
fi

rm -rf ./build_zstd_c
echo "Successfully removed build_zstd_c directory."
