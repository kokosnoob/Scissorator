#!/bin/bash

SRCDIR="$1"

echo $(find "$SRCDIR" -name "*.h" | tr '\n' ' ')
