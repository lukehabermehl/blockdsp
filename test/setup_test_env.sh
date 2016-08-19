#!/bin/bash

git clone https://github.com/google/googletest.git
mkdir -p lib
mkdir -p include
mkdir -p include/gtest

cd googletest && cmake .
make
mv -f googlemock/gtest/libgtest.a googlemock/gtest/libgtest_main.a ../lib
mv -f googletest/include/gtest/* ../include/gtest
cd ../


