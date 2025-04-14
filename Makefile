file_hash: main.c cl_fi.o cl_ui.o linkedlist.o md4.o options.o processfile.o
	gcc main.c cl_fi.o cl_ui.o linkedlist.o md4.o options.o processfile.o -o file_hash

cl_fi.o: cl_fi.c cl_fi.h fi.h global.h
	gcc -c cl_fi.c

cl_ui.o: cl_ui.c cl_ui.h ui.h global.h
	gcc -c cl_ui.c

linkedlist.o: linkedlist.c linkedlist.h global.h
	gcc -c linkedlist.c

md4.o: md4.c md4.h global.h
	gcc -c md4.c

options.o: options.c options.h global.h
	gcc -c options.c
	
processfile.o: processfile.c processfile.h global.h
	gcc -c processfile.c
