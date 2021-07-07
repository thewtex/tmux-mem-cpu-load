#!/bin/bash
set -euo pipefail

source $(dirname "$0")/functions/cmake_fn.sh

cmake_build $@
