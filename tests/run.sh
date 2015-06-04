#!/bin/bash

[[ $DEBUG ]] && set -xv

tm=./interpreter/tm

if ! make; then
	echo "FATAL ERROR make exited with an error" 2>&1
	[[ $DEBUG ]] && set +xv
	exit 1
fi

for i in $(ls ./tests/mila); do
	f=$(echo ${i%.*})

	for O in 0 1; do
		if ! $1 ./bin/mila2tm ./tests/mila/$i -O $O \
		    -o ./tests/tm_comments/${f}.tnyc 2>/dev/null; then
			echo "$f ERROR compilation exited with an error" 2>&1
			continue
		fi

		grep -v "//" ./tests/tm_comments/${f}.tnyc > ./tests/tm/${f}.tny

		if [[ -f ./tests/in/${f}.in ]]; then
			cat <(echo -e "t\np\ng") ./tests/in/${f}.in <(echo "q") \
				> ./tests/in/${f}.in_c
			res=$(cat ./tests/in/${f}.in_c | timeout 20 $tm ./tests/tm/${f}.tny \
				1> ./tests/res/${f}_1.res 2> ./tests/res/${f}_2.res)
		else
			res=$(echo -e "t\np\ng\nq" | timeout 20 $tm ./tests/tm/${f}.tny \
				1> ./tests/res/${f}_1.res 2> ./tests/res/${f}_2.res)
		fi

		if [[ ! -f ./tests/out/${f}.out ]]; then
			echo "$f ERROR no .out file found" 2>&1
			continue
		fi

		steps=$(grep "Number of instructions executed" \
			./tests/res/${f}_1.res)

		if diff ./tests/out/${f}.out <(grep OUT: ./tests/res/${f}_1.res | \
				awk '{ print $2 }' ); then
			echo "\"$f\" (-O=$O) ($steps) OK"
		else
			echo "\"$f\" (-O=$O) (res=$res) FAIL"
		fi
	done
done

[[ $DEBUG ]] && set +xv

