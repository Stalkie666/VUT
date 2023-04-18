#!/bin/bash

echo "Test: Tail"
echo "################################################################"

./tail -n 10 < tests/testTail.txt | tee /tmp/testTail.txt
tail -n 10 tests/testTail.txt > /tmp/tail.txt
if diff /tmp/testTail.txt /tmp/tail.txt
then
    echo "------------------------------------------------------------"
    echo "Vysledek je indenticky s prikazem tail"
fi

echo "################################################################"
./wordcount- < tests/testWordCount.txt | sort > /tmp/originalWordCount.txt
echo "Test: wordcount"
./wordcount < tests/testWordCount.txt | sort | tee /tmp/testWordCount.txt
if diff /tmp/testTail.txt /tmp/tail.txt
then
    echo "------------------------------------------------------------"
    echo "Vystup wordcount je indenticky s vystupem wordcount-"
fi
echo "################################################################"
echo "Test: wordcount-dynamic"
export LD_LIBRARY_PATH="."
./wordcount-dynamic < tests/testWordCount.txt | sort | tee /tmp/testWordCount.txt
if diff /tmp/testTail.txt /tmp/tail.txt
then
    echo "------------------------------------------------------------"
    echo "Vystup wordcount-dynamic je indenticky s vystupem wordcount-"
fi