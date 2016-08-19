#!/bin/bash

cd docs
doxygen Doxyfile

open doxy/html/index.html
