#!/bin/bash

bash run_kslicer.sh
cd example_tracer/shaders_generated/
bash build.sh && cd ../..

mkdir -p cmake-build-release && cd cmake-build-release/
cmake -DCMAKE_CXX_COMPILER=clang++ \
    -DCMAKE_C_COMPILER=clang \
    -DCMAKE_BUILD_TYPE=Release \
    -DUSE_VULKAN=ON ..
make -j 8 && cd ..
