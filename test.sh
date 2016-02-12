#!/bin/bash

testcount=0
testpass=0
failedtests=""

function test {
	testcount=$(($testcount+1))
	./interpreter -i $1
	if [ $? -eq 0 ]; then
		testpass=$(($testpass+1))
	else
		failedtests=$failedtests$1" "
	fi
}

test tests/arithmetic.src

printf "%d/%d tests passed\n" $testpass $testcount
if [ $testpass -eq $testcount ] ; then
	printf "All tests passed!\n"
else
	for testcase in $failedtests ; do
		printf "Failed test %s\n" $testcase
	done
fi
