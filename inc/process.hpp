#ifndef __PROC_HPP__

#define __PROC_HPP__

#include <cstdint>
#include <vector>
#include <sys/types.h>
#include <dirent.h>
#include <fstream>


struct process;
struct thread;

typedef std::vector<process*> proc_vector;
typedef std::vector<thread*> thrd_vector;
typedef std::vector<process*>::iterator proc_vector_iter;
typedef std::vector<thread*>::iterator thrd_vector_iter;

struct process {
	uint32_t pid;

	uint64_t memory_size,
		 rss_size,
		 shared_size,
		 text_size,
		 data_size;

	char state;
	char *pname = new char[50];
	
	thrd_vector* thread_vector = NULL;
};

struct thread {
	uint32_t tid;

	uint64_t memory_size,
		 rss_size,
		 shared_size,
		 text_size, 
		 data_size;
	
	char state;
};

struct memory{
    uint64_t memTotal,
    memFree,
    MemAvailable,
    Buffers,
    Cached;    
};

/** To get all the processes stored in a vector. 
 *  Return the pointer of the vector storing all
 *  the processes.
 **/
proc_vector* get_all_processes();
thrd_vector* get_all_threads(int pid);
void delete_process_vec(proc_vector*);
memory* get_memory_info();

#endif