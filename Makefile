# SPDX-License-Identifier: CC0-1.0
#
# Project: DS Cube Timer
# Author:  nikoi008w 
# Base Makefile template by Antonio Niño Díaz (BlocksDS)

BLOCKSDS	?= /opt/blocksds/core

# User config

NAME		:= DSTimer
GAME_TITLE	:= DSTimer
GAME_SUBTITLE	:= nikoi008w

include $(BLOCKSDS)/sys/default_makefiles/rom_arm9/Makefile
