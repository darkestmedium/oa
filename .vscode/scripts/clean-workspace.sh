#!/bin/bash

cd "/home/$(whoami)/Dropbox/code/oa"

find . -name "Icon?" -print0 | xargs -0 rm -rf
find . -name "desktop.ini" -print0 | xargs -0 rm -rf
find . -name "__pycache__" -print0 | xargs -0 rm -rf
find . -name "__MACOSX" -print0 | xargs -0 rm -rf
find . -name ".DS_Store" -print0 | xargs -0 rm -rf

echo "Finished cleaning workspace from cache and os files."

