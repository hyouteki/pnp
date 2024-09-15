#!/bin/bash

set -e

build_name="pnp"
cc="gcc"
cflags=("-Wall" "-Wextra" "-Wno-format" "-Wno-format-extra-args")
src_dir="./src"
deps=("tomlc99/toml.c")
main_file="main.c"
build_output="./$build_name"

main_path="$src_dir/$main_file"
dep_paths=()
for dep in "${deps[@]}"; do
    dep_paths+=("$src_dir/$dep")
done

$cc $main_path ${cflags[@]} ${dep_paths[@]} -o $build_output
if [ $? -ne 0 ]; then
    echo "error: Build failed with exit code $?"
else
    ./$build_output build
fi
