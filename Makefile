INC_FLAG = -I./inc
DEGUN_FLAG = -g

test_get_all_process: process.o test_get_all_process.o
	g++ ./target/process.o ./target/test_get_all_process.o -o ./target/test_get_all_process

memory_watch:
	g++ $(INC_FLAG) ./target/process.o ./target/main.o -o ./target/memory_watch -lcurses -pthread

process.o:
	g++ $(INC_FLAG) -c ./src/process.cpp -o ./target/process.o

test_get_all_process.o:
	g++ $(INC_FLAG) -c ./test/test_get_all_process.cpp -o ./target/test_get_all_process.o

main.o:
	g++ $(INC_FLAG) -c ./src/main.cpp -lcurses -pthread -o ./target/main.o

run:
	g++ $(INC_FLAG) -c ./src/main.cpp -lcurses -pthread -o ./target/main.o
	g++ $(INC_FLAG) ./target/process.o ./target/main.o -o ./target/memory_watch -lcurses -pthread
	./target/memory_watch

test_get_memory_info:
	g++ $(INC_FLAG) -c ./test/test_get_memory_info.cpp -o ./target/test_get_memory_info.o
	g++ ./target/process.o ./target/test_get_memory_info.o -o ./target/test_get_memory_info

clean:
	rm ./target/*