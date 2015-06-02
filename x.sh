make
./bin/mila2tm -d 10 -o /tmp/lel.tm ./tests/mila/read.mila
cat tests/in/read.in_c | ./interpreter/tm /tmp/lel.tm
