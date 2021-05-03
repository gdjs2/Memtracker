#include <cstdio>
#include "process.hpp"

int main() {
	printf("PID\tSTATUS\t\tNAME\t\tMEMORY\t\tRSS\t\tSHARED\t\tTEXT\t\tDATA\n");
	proc_vector *vec = get_all_processes();
	for (proc_vector_iter it = vec->begin(); it != vec->end(); ++it) {
		printf("%u\t%c\t%16.16s\t%8lu\t%8lu\t%8lu\t%8lu\t%8lu\n",
			(*it)->pid, (*it)->state, (*it)->pname,
			(*it)->memory_size, (*it)->rss_size,
			(*it)->shared_size, (*it)->text_size,
			(*it)->data_size);
	}
	return 0;
}