#!/bin/bash

PIP2="$1"
PYTHON2="$2"
GLAD_SRC="$3"
INCDIR="$4"
GL_MAJOR="$5"
GL_MINOR="$6"
GL_GEN="$7"
GL_EXT="$8"

GLAD_TEMP="temp_glad"

if [[	( ! -f "${INCDIR}/.glad-debug" && "$GL_GEN" == "c-debug" ) || \
		( ! -f "${INCDIR}/.glad-release" && "$GL_GEN" == "c" ) \
]]; then
	rm -rf "${INCDIR}/glad" "${INCDIR}/KHR"
	
	"$PIP2" install --user glad
	"$PYTHON2" -m glad --generator="$GL_GEN" --api="gl=${GL_MAJOR}.${GL_MINOR}" --profile=core --extensions="$GL_EXT" --out-path="$GLAD_TEMP"
	echo "Generated GLAD with Extensions: $GL_EXT"

	# Assemble GLAD dir paths
	mv $GLAD_TEMP/include/* "$INCDIR/"
	mv $GLAD_TEMP/src/* "$GLAD_SRC"

	rm -rf "$GLAD_TEMP"
	
	if [[ "$GL_GEN" == "c-debug" ]]; then
		# Indicate to future builds that GLAD Debug is installed.
		touch "${INCDIR}/.glad-debug"
		rm -f "${INCDIR}/.glad-release"
	elif [[ "$GL_GEN" == "c" ]]; then
		# Indicate to future builds that GLAD Release is installed.
		rm -f "${INCDIR}/.glad-debug"
		touch "${INCDIR}/.glad-release"
	fi
fi
