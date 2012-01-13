#!/bin/bash
./r.sh;
flex -o./out/delcoms.yy.c delcoms.lex; 
flex -o./out/exprs.yy.c exprs.lex; 
cc -o./out/delcoms.out ./out/delcoms.yy.c;
cc -o./out/exprs.out ./out/exprs.yy.c; 
./out/delcoms.out input.txt ./out/input_clean.txt;
./out/exprs.out ./out/input_clean.txt;
