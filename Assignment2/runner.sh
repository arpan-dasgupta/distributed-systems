#!/bin/bash

while :
do
    ./make_test
    erlc 2018111010_2.erl
    erl -noshell -s 2018111010_2 main in.txt output.txt -s init stop > garbage
    ./checker
done;