nesrotom-gen-2015

ukaz rychle vystup ./test.mila
make && ./bin/mila2tm ./test.mila -o test.tny && echo -e "t\np\ng\nq" | ./interpreter/tm ./test.tny  | grep OUT:

