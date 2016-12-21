#!/bin/sh
cmake . -DGUI=off -DSUPPORT_X11=off -DSUPPORT_RPI=on -DSUPPORT_RPI3=on -DCMAKE_BUILD_TYPE=Release .
