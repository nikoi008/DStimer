# SPDX-License-Identifier: CC0-1.0
# SPDX-FileCopyrightText: 2024 Adrian "asie" Siekierka
#
# Modifications and additional logic:
# Copyright (c) 2025 nikoi008
# Licensed under the GNU GPL v3.0
#
# Project: DS Cube Timer
# Author:  nikoi008w 


WONDERFUL_TOOLCHAIN ?= C:/msys64/opt/wonderful
BLOCKSDS            ?= $(WONDERFUL_TOOLCHAIN)/thirdparty/blocksds/core

NAME            := DSTimer
GAME_TITLE      := DSTimer
GAME_SUBTITLE   := DS speedcubing timer
GAME_AUTHOR     := nikoi008          
GAME_ICON       := icon.png #credits to u/averageteencuber (reddit) for his art

include $(BLOCKSDS)/sys/default_makefiles/rom_arm9/Makefile
