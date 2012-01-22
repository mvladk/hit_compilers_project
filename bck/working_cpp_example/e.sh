#!/bin/bash

./r.sh;
##flex -o./out/delcoms.yy.c delcoms.lex; 
clear;
echo "FLEX-------------------------------------";
flex -o./out/exprs.yy.c exprs.lex;
echo "YACC-------------------------------------";
bison -o./out/y.exprs.c -d exprs.yacc;  
echo "c++-------------------------------------";
##c++ -o./out/delcoms.out ./out/delcoms.yy.c;
c++ -o./out/exprs.out ./out/exprs.yy.c ./out/y.exprs.c; 
##./out/delcoms.out input.txt ./out/input_clean.txt;
echo "RUN=====================================";
./out/exprs.out < input.txt;
