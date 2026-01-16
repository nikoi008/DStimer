# DStimer
A rubik's cube timer for the Nintendo DS

# Key features
Scramble preview on bottom screen  
Transition between scramble and timing similar to that of popular speedcubing timers.  
Scrambling that works smartly, preventing redundant moves such as R R'.  
Solve saving and display so that you can track your progress  

# Controls
| Key    | Action |
| -------- | ------- |
| Dpad down  | Scroll down on previously saved times   |
| Dpad up    | Scroll up     |
| Dpad left  | Jumps to the oldest solve    |
| Dpad right | Jumps to the newest solve |
| B          | Deletes current solve  |
| Touchscreen / any other button | Starts/stops timer |

# Installation  
Go to the [Releases](https://github.com/nikoi008/DStimer/releases/) and pop it in your flashcart or your emulator of choice  

# Building for yourself
This project uses [BlocksDS](https://github.com/blocksds/sdk) SDK, and [Wonderful Toolchain](https://wonderful.asie.pl/wiki/doku.php?id=getting_started).  
Detailed installation instructions can be found in the [Getting Started](https://blocksds.skylyrac.net/docs/setup/) section in the BlocksDS docs.

Then put the source folder and makefile somewhere in /opt/wonderful/thirdparty/blocksds/core/ or C\msys64\opt\wonderful\thirdparty\blocksds\core on Windows to ensure no missing dependency errors.

Open wonderful and navigate to the directory `cd /PATH/TO/YOUR/MAKEFILE` and then type `make`.

# Possible upcoming features  
> Please keep in mind that this is a hobby project and I will likely not be implementing many of these soon, however feel free to fork it if you want to improve it.

Since this is something that I will use daily, I have created a checklist with things that I want to add to this project to make it more enjoyable to use, such as:  
✓ File handling - Creating sessions and save times that will be stored on the DS.  
AOX - Making an average of n solves to track progress  
Scrambling beyond 3x3 - Introduce different scrambling for 2x2-7x7 and even more obscure cubes such as Square-1. Cubes available currently: 2x2,3x3  
More customisation - Ability to change colour of background, cube stickers and possibly fonts.

# Credits
Credits to u/averageteencuber for the cube drawing that is now the icon of this project (used with permission)
# Disclaimer
Starting with version 3.0.0, DStimer is officially licensed under the [GNU GPL v3.0.](LICENSE) Previous versions were released without an explicit license and are provided for historical purposes only.


