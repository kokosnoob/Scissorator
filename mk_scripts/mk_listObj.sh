#!/bin/bash

SRCDIR="$1"

echo $(find "src/" -name "*.cpp" | xargs -I{} basename {} | sed '/$/s/^/obj\//' | sed 's/cpp/o/g' | tr '\n' ' ')
