#!/bin/bash

[[ $DEBUG ]] && set -xv

tm=./interpreter/tm

if ! make; then
	echo "FATAL ERROR make exited with an error" 2>&1
	[[ $DEBUG ]] && set +xv
	return
fi

for i in $(ls ./tests/mila); do
	f=$(echo ${i%.*})

	if ! ./bin/mila2tm ./tests/mila/$i -o ./tests/tm_comments/${f}.tnyc; then
		echo "$f ERROR compilation exited with an error" 2>&1
		continue
	fi

	grep -v "//" ./tests/tm_comments/${f}.tnyc > ./tests/tm/${f}.tny

	if [[ -f ./tests/in/${f}.in ]]; then
		cat <(echo "g") ./tests/tm/${f}.tny <(echo "q") \
			> ./tests/tm_vm/${f}.tny
		res=$(cat ./tests/in/${f}.in | $tm ./tests/tm_vm/${f}.tny)
	else
		res=$(echo -e "t\np\ng\nq" | $tm ./tests/tm/${f}.tny \
			1> ./tests/res/${f}_1.res 2> ./tests/res/${f}_2.res)
	fi

	if [[ ! -f ./tests/out/${f}.out ]]; then
		echo "$f ERROR no .out file found" 2>&1
		continue
	fi

	if diff ./tests/out/${f}.out <(grep OUT: ./tests/res/${f}_1.res | \
			awk '{ print $2 }'  ); then
		echo "$f OK"
	else
		echo "$f FAIL res=$res"
	fi
done

[[ $DEBUG ]] && set +xv

