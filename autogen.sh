#!/bin/sh

libtoolize &&
autoconf -i &&
automake --add-missing &&
./configure $@
