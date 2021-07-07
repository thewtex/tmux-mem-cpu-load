#!/bin/bash
set -euo pipefail

curl https://raw.githubusercontent.com/dockcross/dockcross/master/Makefile -o dockcross-Makefile
make -f dockcross-Makefile display_images

source $(dirname "$0")/functions/cmake_fn.sh

for image in $(make -f dockcross-Makefile display_images); do
    cmake_build $image $@
done
