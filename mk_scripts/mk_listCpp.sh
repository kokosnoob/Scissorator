#!/bin/bash

SRCDIR="$1"

echo $(find "$SRCDIR" -name "*.cpp" | tr '\n' ' ')
