#include <cstdio>
#include <unistd.h>
#include "process.hpp"

int main() {
	printf("PID\tSTATUS\t\tNAME\t\tMEMORY\t\tRSS\t\tSHARED\t\tTEXT\t\tDATA\n");

	proc_vector *vec;

	while(1) {
		sleep(1);
		vec = get_all_processes();
		for (proc_vector_iter it = vec->begin(); it != vec->end(); ++it) {
		printf("%u\t%c\t%16.16s\t%8lu\t%8lu\t%8lu\t%8lu\t%8lu\n",
			(*it)->pid, (*it)->state, (*it)->pname,
			(*it)->memory_size, (*it)->rss_size,
			(*it)->shared_size, (*it)->text_size,
			(*it)->data_size);
		if ((*it)->thread_vector->size() != 1
			|| (*it)->thread_vector->at(0)->tid != (*it)->pid) {

				thrd_vector *thread_vector = (*it)->thread_vector;
				for (thrd_vector_iter it = thread_vector->begin(); it != thread_vector->end(); ++it) {
					printf("|____%u\t%c\t%8lu\t%8lu\t%8lu\t%8lu\t%8lu\n",
						(*it)->tid, (*it)->state,
						(*it)->memory_size, (*it)->rss_size,
						(*it)->shared_size, (*it)->text_size,
						(*it)->data_size);
				}


			}
		}
		delete_process_vec(vec);
	}
	

	
	return 0;
}