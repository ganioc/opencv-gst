#!/bin/bash
g++ run.cc -o run `PKG_CONFIG_PATH=/opt/ruff/lib/pkgconfig/ pkg-config --cflags --libs opencv4`
