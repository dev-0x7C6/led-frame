#!/bin/bash 
cd `git rev-parse --show-toplevel`
doxygen documentation/project.doxy
xdg-open documentation/doxygen/html/index.html
