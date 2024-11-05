#!/bin/bash

set -xe

CC="gcc"
CFLAGS=("-Wall" "-Wextra" "-Wno-format" "-Wno-format-extra-args" "-Wno-unused-result" "-O5")
SRC_DIR="./src"
DEPS=("tomlc99/toml.c")

BUILD_NAME="pnp"
MAIN_FILE="main.c"
MAIN_PATH="${SRC_DIR}/${MAIN_FILE}"

DEP_PATHS=()
for DEP in "${DEPS[@]}"; do
    DEP_PATHS+=("${SRC_DIR}/${DEP}")
done

$CC $MAIN_PATH ${DEP_PATHS[@]} ${CFLAGS[@]} -o $BUILD_NAME
./$BUILD_NAME build
