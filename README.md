===warning: this code is really ultraterrible and nobody should EVER try to read it.===

----------------

nesrotom-gen-2015

- YES - Překladač vytváří korektní cílový kód z pohledu funkčnosti a používá optimalizaci výpočtu numerických konstantních výrazu (constant folding) (pozn. tato optimalizace je již implementována v dodaném front endu jazyka Mila) - max 10 b.

- YES - Optimalizujte společné podvýrazy v rámci jednoho výrazu. - max 5 b.

- YES - Optimalizujte společné podvýrazy i mezi výrazy. - max 5 b. (uvnitr statm)

- YES - Vyhodnocení výrazů v čase kompilace - max 5 b.

- NO - Optimalizujte používání cyklů přesunutím invariantů mimo tělo cyklu - max 5 b.

tests (18:46:00 4.6.2015)

	n@t:~/MEGA/github/nesrotom-gen-2015$ time ./tests/run.sh
	g++ -Ofast -Wall -Wextra -Wformat-security -Wshadow -Wlogical-op -Wfloat-equal -Werror -pedantic -std=c++11 -c -o ./bin/debug.o src/debug.cpp 
	g++ -Ofast -Wall -Wextra -Wformat-security -Wshadow -Wlogical-op -Wfloat-equal -Werror -pedantic -std=c++11 -c -o ./bin/strom.o src/strom.cpp 
	g++ -Ofast -Wall -Wextra -Wformat-security -Wshadow -Wlogical-op -Wfloat-equal -Werror -pedantic -std=c++11 -c -o ./bin/lexan.o src/lexan.cpp 
	g++ -Ofast -Wall -Wextra -Wformat-security -Wshadow -Wlogical-op -Wfloat-equal -Werror -pedantic -std=c++11 -c -o ./bin/parser.o src/parser.cpp 
	g++ -Ofast -Wall -Wextra -Wformat-security -Wshadow -Wlogical-op -Wfloat-equal -Werror -pedantic -std=c++11 -c -o ./bin/code.o src/code.cpp 
	g++ -Ofast -Wall -Wextra -Wformat-security -Wshadow -Wlogical-op -Wfloat-equal -Werror -pedantic -std=c++11 -c -o ./bin/tabsym.o src/tabsym.cpp 
	g++ -Ofast -Wall -Wextra -Wformat-security -Wshadow -Wlogical-op -Wfloat-equal -Werror -pedantic -std=c++11 -c -o ./bin/vstup.o src/vstup.cpp 
	g++ -Ofast -Wall -Wextra -Wformat-security -Wshadow -Wlogical-op -Wfloat-equal -Werror -pedantic -std=c++11 -c -o ./bin/main.o src/main.cpp 
	g++ -Ofast -Wall -Wextra -Wformat-security -Wshadow -Wlogical-op -Wfloat-equal -Werror -pedantic -std=c++11 ./bin/main.o \
		./bin/debug.o ./bin/strom.o ./bin/lexan.o ./bin/parser.o ./bin/code.o ./bin/tabsym.o ./bin/vstup.o -o ./bin/mila2tm 
	valgrind=""
	"big_program" (-O=0) (Number of instructions executed = 110) OK
	"big_program" (-O=1) (Number of instructions executed = 110) OK
	"bop_2" (-O=0) (Number of instructions executed = 7) OK
	"bop_2" (-O=1) (Number of instructions executed = 5) OK
	"bop_2var" (-O=0) (Number of instructions executed = 11) OK
	"bop_2var" (-O=1) (Number of instructions executed = 9) OK
	"bop_2x2" (-O=0) (Number of instructions executed = 13) OK
	"bop_2x2" (-O=1) (Number of instructions executed = 9) OK
	"bop_3" (-O=0) (Number of instructions executed = 9) OK
	"bop_3" (-O=1) (Number of instructions executed = 5) OK
	"bop_4" (-O=0) (Number of instructions executed = 11) OK
	"bop_4" (-O=1) (Number of instructions executed = 5) OK
	"bops" (-O=0) (Number of instructions executed = 171) OK
	"bops" (-O=1) (Number of instructions executed = 171) OK
	"edux_1a" (-O=0) (Number of instructions executed = 97) OK
	"edux_1a" (-O=1) (Number of instructions executed = 37) OK
	"edux_noread" (-O=0) (Number of instructions executed = 351) OK
	"edux_noread" (-O=1) (Number of instructions executed = 33) OK
	"edux_read" (-O=0) (Number of instructions executed = 367) OK
	"edux_read" (-O=1) (Number of instructions executed = 137) OK
	"gcd" (-O=0) (Number of instructions executed = 90) OK
	"gcd" (-O=1) (Number of instructions executed = 90) OK
	"if_then_else" (-O=0) (Number of instructions executed = 25) OK
	"if_then_else" (-O=1) (Number of instructions executed = 25) OK
	"if_then" (-O=0) (Number of instructions executed = 15) OK
	"if_then" (-O=1) (Number of instructions executed = 15) OK
	"nothing" (-O=0) (Number of instructions executed = 11) OK
	"nothing" (-O=1) (Number of instructions executed = 9) OK
	"optimize_1" (-O=0) (Number of instructions executed = 131) OK
	"optimize_1" (-O=1) (Number of instructions executed = 21) OK
	"read" (-O=0) (Number of instructions executed = 23) OK
	"read" (-O=1) (Number of instructions executed = 23) OK
	"rewrite2" (-O=0) (Number of instructions executed = 31) OK
	"rewrite2" (-O=1) (Number of instructions executed = 31) OK
	"rewrite" (-O=0) (Number of instructions executed = 41) OK
	"rewrite" (-O=1) (Number of instructions executed = 41) OK
	"stack" (-O=0) (Number of instructions executed = 43) OK
	"stack" (-O=1) (Number of instructions executed = 5) OK
	"two_bops_with_dups" (-O=0) (Number of instructions executed = 23) OK
	"two_bops_with_dups" (-O=1) (Number of instructions executed = 23) OK
	"while_const2" (-O=0) (Number of instructions executed = 151) OK
	"while_const2" (-O=1) (Number of instructions executed = 151) OK
	"while_const" (-O=0) (Number of instructions executed = 115) OK
	"while_const" (-O=1) (Number of instructions executed = 115) OK
	"while_if" (-O=0) (Number of instructions executed = 248) OK
	"while_if" (-O=1) (Number of instructions executed = 248) OK
	"while" (-O=0) (Number of instructions executed = 79) OK
	"while" (-O=1) (Number of instructions executed = 79) OK
	valgrind="valgrind"
	"big_program" (-O=0) (Number of instructions executed = 110) OK
	"big_program" (-O=1) (Number of instructions executed = 110) OK
	"bop_2" (-O=0) (Number of instructions executed = 7) OK
	"bop_2" (-O=1) (Number of instructions executed = 5) OK
	"bop_2var" (-O=0) (Number of instructions executed = 11) OK
	"bop_2var" (-O=1) (Number of instructions executed = 9) OK
	"bop_2x2" (-O=0) (Number of instructions executed = 13) OK
	"bop_2x2" (-O=1) (Number of instructions executed = 9) OK
	"bop_3" (-O=0) (Number of instructions executed = 9) OK
	"bop_3" (-O=1) (Number of instructions executed = 5) OK
	"bop_4" (-O=0) (Number of instructions executed = 11) OK
	"bop_4" (-O=1) (Number of instructions executed = 5) OK
	"bops" (-O=0) (Number of instructions executed = 171) OK
	"bops" (-O=1) (Number of instructions executed = 171) OK
	"edux_1a" (-O=0) (Number of instructions executed = 97) OK
	"edux_1a" (-O=1) (Number of instructions executed = 37) OK
	"edux_noread" (-O=0) (Number of instructions executed = 351) OK
	"edux_noread" (-O=1) (Number of instructions executed = 33) OK
	"edux_read" (-O=0) (Number of instructions executed = 367) OK
	"edux_read" (-O=1) (Number of instructions executed = 137) OK
	"gcd" (-O=0) (Number of instructions executed = 90) OK
	"gcd" (-O=1) (Number of instructions executed = 90) OK
	"if_then_else" (-O=0) (Number of instructions executed = 25) OK
	"if_then_else" (-O=1) (Number of instructions executed = 25) OK
	"if_then" (-O=0) (Number of instructions executed = 15) OK
	"if_then" (-O=1) (Number of instructions executed = 15) OK
	"nothing" (-O=0) (Number of instructions executed = 11) OK
	"nothing" (-O=1) (Number of instructions executed = 9) OK
	"optimize_1" (-O=0) (Number of instructions executed = 131) OK
	"optimize_1" (-O=1) (Number of instructions executed = 21) OK
	"read" (-O=0) (Number of instructions executed = 23) OK
	"read" (-O=1) (Number of instructions executed = 23) OK
	"rewrite2" (-O=0) (Number of instructions executed = 31) OK
	"rewrite2" (-O=1) (Number of instructions executed = 31) OK
	"rewrite" (-O=0) (Number of instructions executed = 41) OK
	"rewrite" (-O=1) (Number of instructions executed = 41) OK
	"stack" (-O=0) (Number of instructions executed = 43) OK
	"stack" (-O=1) (Number of instructions executed = 5) OK
	"two_bops_with_dups" (-O=0) (Number of instructions executed = 23) OK
	"two_bops_with_dups" (-O=1) (Number of instructions executed = 23) OK
	"while_const2" (-O=0) (Number of instructions executed = 151) OK
	"while_const2" (-O=1) (Number of instructions executed = 151) OK
	"while_const" (-O=0) (Number of instructions executed = 115) OK
	"while_const" (-O=1) (Number of instructions executed = 115) OK
	"while_if" (-O=0) (Number of instructions executed = 248) OK
	"while_if" (-O=1) (Number of instructions executed = 248) OK
	"while" (-O=0) (Number of instructions executed = 79) OK
	"while" (-O=1) (Number of instructions executed = 79) OK
	oks=96 fails=0

	real	0m45.148s
	user	0m41.577s
	sys	0m3.775s

