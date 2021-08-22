#!/bin/bash

set -ex
mkdir -p include/roms

for filename in $(find -name "*.gb") $(find -name "*.gbc") $(find -name "*.bin"); do
	xxd -i ${filename} > include/roms/${filename}.h
done
