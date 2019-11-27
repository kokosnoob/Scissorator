# Macros - TOOLCHAIN can be gcc, clang, or windows
SHELL		= /bin/bash
DATE		= $(shell date)
LIBS_NAME	= libs



# Basic Configuration
TARGET		= learn
TOOLCHAIN	= gcc



# Project Dirs
SRCDIR		= src
SHADIR		= src/shaders
RECDIR		= resources

LIBDIR		= $(LIBS_NAME)/extern
INCDIR		= $(LIBS_NAME)/includes
DYNDIR		= $(LIBS_NAME)/dynamic

OBJDIR		= obj
BIN			= bin
SHABIN		= $(BIN)/shaders
RECBIN		= $(BIN)/resources
LIBBIN		= $(BIN)/$(LIBS_NAME)

MKSCRIPTS	= mk_scripts



# External Build File Paths
TARGETPATH	= $(BIN)/$(TARGET)



# Build Assistance
PYTHON2		= python2
PIP2		= pip2

MK_GLAD		= $(MKSCRIPTS)/mk_glad.sh

MK_LISTCPP	= $(MKSCRIPTS)/mk_listCpp.sh
MK_LISTH	= $(MKSCRIPTS)/mk_listH.sh
MK_LISTOBJ	= $(MKSCRIPTS)/mk_listObj.sh



# OpenGL Info - version & extensions
GL_MAJOR	= 3
GL_MINOR	= 3
GL_EXT		= ""



# Pure C Flags - common C compiler options
CCSTD		= -std=c99
CCOPS		= -flto=full -Ofast -ffast-math



# C++ Flags - common C++ compiler options
VARS		= -D APP_GL_MAJOR_VERSION=$(GL_MAJOR) -D APP_GL_MINOR_VERSION=$(GL_MAJOR)

CXXERRS		=	-pedantic -Wall -Weffc++ -Wextra -Werror -Wcast-align -Wcast-qual -Wctor-dtor-privacy -Wdisabled-optimization\
				-Winit-self -Wmissing-declarations -Wmissing-include-dirs -Woverloaded-virtual -Wformat=2 -Wold-style-cast  \
				-Wredundant-decls -Wsign-promo -Wstrict-overflow=5  -Wno-unused -Wundef -Winline -Winvalid-pch -Wshadow \
				-Wswitch-default -Wnull-dereference -Wsign-conversion -Wno-unused-parameter

CXXSTD		= -std=c++17
CXXOPS		= -fopenmp -flto=full -Ofast -ffast-math



# Linker Flags - common linker options
LIBS		= -lm -lstdc++ -lglfw -lOpenImageIO -lassimp

LOPTS		= -Wl,-rpath,'$$ORIGIN/$(LIBS_NAME)' -Wl,-rpath-link,./$(DYNDIR) -L./$(DYNDIR) -fopenmp -flto=full -Ofast -ffast-math -Wstringop-overflow=0


# GCC Toolchain - comment this block if not using gcc
CC = gcc-8
CXX = g++-8
LINKER = g++-8

CXXERRS += -Wno-maybe-uninitialized -Wlogical-op -Wnoexcept -Wstrict-null-sentinel -Wuseless-cast

CXXOPTS += -D_GLIBCXX_PARALLEL

LOPTS += -fuse-ld=gold -no-pie





# Key Flags
CFLAGS		= -isystem $(INCDIR) $(CCSTD) $(CCOPS)
#CXXFLAGS	= $(CXXSTD) $(CXXERRS) -isystem $(INCDIR) -I$(SRCDIR) $(VARS) $(CXXOPTS)
CXXFLAGS	= $(CXXSTD) $(CXXERRS) -I$(SRCDIR) -I$(INCDIR) $(VARS) $(CXXOPTS)
LFLAGS		= $(LOPTS) $(LIBS)



#Internal Vars
SOURCES		:= $(shell $(MK_LISTCPP) $(SRCDIR))
INCLUDES	:= $(shell $(MK_LISTH) $(SRCDIR))
OBJECTS		:= $(addprefix $(OBJDIR)/,$(SOURCES:$(SRCDIR)/%.cpp=%.o))



# GLAD Generation Vars
GL_GEN		= c
GLAD_SRC	= $(INCDIR)/glad/glad.c



#Main Rules
.PHONY: all debug release run

all: debug

run: all
	@cd $(BIN); ./$(TARGET); cd - > /dev/null



# Release Targets
.PHONY: debug release build

debug: CXXFLAGS += -DDEBUG -g -ggdb
#~ debug: CXXFLAGS += -fsanitize=address -fno-omit-frame-pointer
#~ debug: LFLAGS += -fsanitize=address -fno-omit-frame-pointer
debug: GL_GEN = c-debug

debug: build
release: build
	@strip --strip-all $(BIN)/$(TARGET)



# Build Targets
build: $(TARGETPATH) resources shaders


# Housekeeping Rules
.PHONY: clean clean-glad distclean

clean:
	-rm -rf $(OBJDIR) $(BIN)

distclean: unsetup




# Resource Rules
.PHONY: shaders resources dynamic

shaders: dir-setup
	@cp -r $(SHADIR)/* $(SHABIN)

resources: dir-setup
	@cp -r $(RECDIR)/* $(RECBIN)



# Setup and Dependencies
.PHONY: setup unsetup dir-setup deps-setup lib-setup glad-setup

setup: deps-setup glad-setup

unsetup: clean clean-glad
	-rm -rf $(OBJDIR) $(BIN) $(DYNDIR) $(INCDIR)
	@if $(PYTHON2) -c "import glad" &> /dev/null; then yes y | $(PIP2) uninstall glad --quiet; echo "y"; fi

dir-setup: $(DYNDIR) $(INCDIR) $(OBJDIR) $(BIN) $(SHABIN) $(RECBIN)

$(DYNDIR):
	@mkdir -p $(DYNDIR)

$(INCDIR):
	@mkdir -p $(INCDIR)

$(OBJDIR):
	@mkdir -p $(OBJDIR)

$(BIN):
	@mkdir -p $(BIN)

$(SHABIN):
	@mkdir -p $(SHABIN)

$(RECBIN):
	@mkdir -p $(RECBIN)


deps-setup:
	@$(MK_DEPS) $(DEPSPATH)

glad-setup:
	@$(MK_GLAD) $(PIP2) $(PYTHON2) $(GLAD_SRC) $(INCDIR) $(GL_MAJOR) $(GL_MINOR) $(GL_GEN) $(GL_EXT)
	



# Source Rules (code generation)
$(GLAD_SRC): glad-setup



# Object Rules - setup must always run first, without forcing a target update; thus the | .
$(OBJECTS): | setup dir-setup
$(OBJDIR)/glad.o: | setup dir-setup

$(OBJDIR)/glad.o: $(GLAD_SRC)
	-$(CC) $(CFLAGS) -w -c $(GLAD_SRC) -o $(OBJDIR)/glad.o

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	@mkdir -p $(shell dirname $@)
	-$(CXX) $(CXXFLAGS) -c $< -o $@



# Linker Rules
$(TARGETPATH): $(OBJDIR)/glad.o $(OBJECTS)
	-$(LINKER) $(OBJECTS) $(OBJDIR)/glad.o $(LFLAGS) -o $@
	@echo "Successfully linked "$@" at "$(shell date)
