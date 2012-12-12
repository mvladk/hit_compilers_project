..\vs2010\mvcompiler\Debug\mvcompiler.exe < semantic_error_1.txt
copy log.txt semantic_error_1.log
..\vs2010\mvcompiler\Debug\mvcompiler.exe < semantic_error_2.txt
copy log.txt semantic_error_2.log

..\vs2010\mvcompiler\Debug\mvcompiler.exe < input_correct.txt
copy log.txt input_correct.log
..\vs2010\mvcompiler\Debug\mvcompiler.exe < input_correct_2.txt
copy log.txt input_correct_2.log

..\vs2010\mvcompiler\Debug\mvcompiler.exe < input_incorrect_1.txt
copy log.txt input_incorrect_1.log
..\vs2010\mvcompiler\Debug\mvcompiler.exe < input_incorrect_2.txt
copy log.txt input_incorrect_2.log

..\vs2010\mvcompiler\Debug\mvcompiler.exe < input_incorrect_lex.txt
copy log.txt input_incorrect_lex.log
