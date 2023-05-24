#!/bin/bash
g++ main.cpp `PKG_CONFIG_PATH=/opt/ruff/lib/pkgconfig/ pkg-config --cflags --libs opencv4`
