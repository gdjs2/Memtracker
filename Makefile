INC_FLAG = -I./inc
DEGUN_FLAG = -g

test_get_all_process: create_dir process.o test_get_all_process.o
	g++ ./target/process.o ./target/test_get_all_process.o -o ./target/test_get_all_process

create_dir:
	mkdir target
process.o:
	g++ $(INC_FLAG) -c ./src/process.cpp -o ./target/process.o

test_get_all_process.o:
	g++ $(INC_FLAG) -c ./test/test_get_all_process.cpp -o ./target/test_get_all_process.o

clean:
	rm ./target/*