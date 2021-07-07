#!/bin/bash
set -euo pipefail

source $(dirname "$0")/functions/makefile_fn.sh

makefile_build $@
