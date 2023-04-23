#!/bin/bash
echo "----------------------------TESTOVANI---------------------------"
# Testy pro tail
echo "Test: Tail"
./tail -n 10 < tests/testTail.txt | tee /tmp/testTail.txt
tail -n 10 tests/testTail.txt > /tmp/tail.txt
echo "----------------------------------------------------------------"
if diff /tmp/testTail.txt /tmp/tail.txt
then
    echo "Vysledek je indenticky s prikazem tail"
else
    echo "Neodpovida testovacimu vystupu"
fi

echo "################################################################"

# Referencni reseni wordcount
./wordcount- < tests/testWordCount.txt | sort > /tmp/originalWordCount.txt

# Test pro staticky wordcount
echo "Test: wordcount"
./wordcount < tests/testWordCount.txt | sort | tee /tmp/testWordCount.txt
    echo "----------------------------------------------------------------"
if diff /tmp/testTail.txt /tmp/tail.txt
then
    echo "Vystup wordcount je indenticky s vystupem wordcount-"
else
    echo "Neodpovida testovacimu vystupu"
fi

# Test pro dynamicky wordcount
echo "################################################################"
echo "Test: wordcount-dynamic"
export LD_LIBRARY_PATH="."
./wordcount-dynamic < tests/testWordCount.txt | sort | tee /tmp/testWordCount.txt
echo "----------------------------------------------------------------"
if diff /tmp/testTail.txt /tmp/tail.txt
then
    echo "Vystup wordcount-dynamic je indenticky s vystupem wordcount-"
else
    echo "Neodpovida testovacimu vystupu"
fi