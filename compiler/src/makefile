SHELL := /bin/bash  # Use bash
compile:
	gcc -o compiler compiler.c io_utils.c scanner.c trie.c parser.c ast.c generate.c

sanit:
	gcc compiler.c io_utils.c scanner.c trie.c parser.c ast.c -o compilerSan -fsanitize=address -g

testsanit: 
	./compilerSan < test.txt

testing: 
	./compiler < test.txt