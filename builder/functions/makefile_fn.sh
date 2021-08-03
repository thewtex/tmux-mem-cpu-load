#!/bin/bash
set -euo pipefail

makefile_build () {
    local image=$1
    shift 1

    echo "Pulling dockcross/$image"
    docker pull dockcross/$image
    echo "Make script dockcross-$image"
    docker run --rm dockcross/$image > ./dockcross-$image
    chmod +x ./dockcross-$image
    echo "Build..."
    ./dockcross-$image bash -c 'make CXX=$CXX CC=$CC AR=$AR AS=$AS CPP=$CPP FC=$FC'
}
