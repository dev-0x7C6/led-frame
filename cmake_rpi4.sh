#!/bin/sh
cmake . -DSUPPORT_X11=off -DSUPPORT_DISPMANX=on -DOPTIMIZE_RPI4=on -DCMAKE_BUILD_TYPE=Release .