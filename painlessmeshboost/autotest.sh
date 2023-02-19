#!/bin/bash

inotify-hookable -w src -w test -f CMakeLists.txt -c "rm bin/*; cmake . -DCMAKE_CXX_FLAGS=\"-Wall -Werror\"; make -j4; run-parts --regex test bin/"
