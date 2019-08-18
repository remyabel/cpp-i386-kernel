#!/bin/bash

set -eu

declare -a ignore=( -path ./.git -o -path ./build-output -o -path ./test/build-output )

find . \( "${ignore[@]}" \) -prune -o \
       \( -name 'CMakeLists.txt' -o -name '*.cmake' \) \
       -exec cmake-format -i '{}' \;

find . \( "${ignore[@]}" \) -prune -o \
    \( -name '*.hpp' -o -name '*.h' \) \
    -exec clang-format -i '{}' \;

find . \( "${ignore[@]}" \) -prune -o \
       -iname '*.sh' -exec shellcheck \
         --exclude=SC1090 \
         --exclude=SC2034 \
         --exclude=SC2153 \
         --exclude=SC2181 \
       '{}' \;

git diff --ignore-submodules=all --color --exit-code .
