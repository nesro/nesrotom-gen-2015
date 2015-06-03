#!/bin/bash

[[ $DEBUG ]] && set -xv

tm=./interpreter/tm

if ! make; then
	echo "FATAL ERROR make exited with an error" 2>&1
	[[ $DEBUG ]] && set +xv
	exit 1
fi

#if ! ./bin/mila2tm ./tests/mila/${1}.mila -o /tmp/lel.tm; then
if ! ./bin/mila2tm -d $3 -O $2 $1 -o /tmp/lel.tm; then
	echo "$f ERROR compilation exited with an error" 2>&1
	[[ $DEBUG ]] && set +xv
	exit 1
fi
cat /tmp/lel.tm
echo -e "t\np\ng\nq" | $tm /tmp/lel.tm


