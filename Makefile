INC_FLAG = -I./inc
DEGUN_FLAG = -g
CC = g++
PTHREAD = -lpthread

test_get_all_process: create_dir process.o test_get_all_process.o
	$(CC) ./target/process.o ./target/test_get_all_process.o -o ./target/test_get_all_process --static

create_dir:
	test -d target || mkdir target

memory_watch: process.o main.o
	$(CC) $(INC_FLAG) ./target/process.o ./target/main.o -o ./target/memory_watch -lcurses -pthread

process.o:
	$(CC) $(INC_FLAG) -c ./src/process.cpp -o ./target/process.o --static

test_get_all_process.o:
	$(CC) $(INC_FLAG) -c ./test/test_get_all_process.cpp -o ./target/test_get_all_process.o --static

multi_thread_program: create_dir
	$(CC) $(INC_FLAG) ./test/multi_thread_program.cpp -o ./target/multi_thread_program $(PTHREAD)

main.o:
	$(CC) $(INC_FLAG) -c ./src/main.cpp -lcurses -pthread -o ./target/main.o

run: ./src/main.cpp process.o 
	$(CC) $(INC_FLAG) -c ./src/main.cpp -lcurses -pthread -o ./target/main.o
	$(CC) $(INC_FLAG) ./target/process.o ./target/main.o -o ./target/memory_watch -lcurses -pthread
	./target/memory_watch

test_get_memory_info:
	$(CC) $(INC_FLAG) -c ./test/test_get_memory_info.cpp -o ./target/test_get_memory_info.o
	$(CC) ./target/process.o ./target/test_get_memory_info.o -o ./target/test_get_memory_info

clean:
	rm -rf ./target