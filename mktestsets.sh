#!/bin/bash

for d in */; do
	if [ -d "$d" ]; then
		cd $d
		echo "moving testsets in $d ..."
		mkdir testsets
		rm -f a.out && echo "  - removed a.out ..."
		for f in *.in; do
			mv "$f" testsets && echo "  - moved $f ..."
		done
		for f in *.out; do
			mv "$f" testsets && echo "  - moved $f ..."
		done
		cd ..
	fi
done
