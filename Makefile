INC_FLAG = -I./inc
DEGUN_FLAG = -g
CC = g++
PTHREAD = -lpthread

test_get_all_process: create_dir process.o test_get_all_process.o
	$(CC) ./target/process.o ./target/test_get_all_process.o -o ./target/test_get_all_process

create_dir:
	test -d target || mkdir target

process.o:
	$(CC) $(INC_FLAG) -c ./src/process.cpp -o ./target/process.o

test_get_all_process.o:
	$(CC) $(INC_FLAG) -c ./test/test_get_all_process.cpp -o ./target/test_get_all_process.o

multi_thread_program: create_dir
	$(CC) $(INC_FLAG) ./test/multi_thread_program.cpp -o ./target/multi_thread_program $(PTHREAD)

clean:
	rm -rf ./target