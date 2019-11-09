#!/bin/bash

while [[ $# -gt 0 ]]
do
key="$1"

case $key in
    -d|--show-diff)
    SHOW_DIFF="$2"
    shift # past argument
    shift # past value
    ;;
    *)    # unknown option
    EXEC="$1" # save it in an array for later
    shift # past argument
    ;;
esac
done

if ! [ -z "$EXEC" ]; then
	if ! [ -d "testsets" ]; then
		echo "No testsets found."
		exit 1
	fi
	for test in testsets/*.in; do
		expected="testsets/$(basename $test .in).out"
		echo "Comparint $test to $expected"
		if ./$EXEC < $test | diff $expected -; then
			echo "Test failed yo"
		else
			echo "Test succeeded"
		fi
	done
fi

