# SPDX-License-Identifier: CC0-1.0
#
# SPDX-FileContributor: Antonio Niño Díaz, 2024

BLOCKSDS	?= /opt/blocksds/core

# User config
NAME            := DSTimer
GAME_TITLE      := DSTimer
GAME_SUBTITLE   := DS speedcubing timer
GAME_AUTHOR     := nikoi008          
GAME_ICON       := icon.png #credits to u/averageteencuber (reddit) for his art

include $(BLOCKSDS)/sys/default_makefiles/rom_arm9/Makefile
