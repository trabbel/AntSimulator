#!/bin/bash
module load cmake
module load rhash/gcc-8.2.0/1.3.5

set -e


mkdir -p build && cd build
cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Release ..
make
