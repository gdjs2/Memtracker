#include "process.hpp"
#include <cstdio>
#include <ctype.h>
#include <cstdlib>
#include <dirent.h>
#include <fstream>
#include <algorithm>
#include <cstring>
using namespace std;


/** To check whether a folder is a pid folder.
 *  Return true if it is, else false. This is
 *  a inner function only.
 **/
bool is_number_folder(dirent* entry) {

	const char* p = entry->d_name;
	while (*p) {
		if (!isdigit(*p)) return 0;
		++p;
	}
	return 1;
}

/**
 * @brief Get the all processes object
 *
 * @return proc_vector* , a vector which contains memory usage info a process
 */
proc_vector* get_all_processes() {
	DIR* dp = opendir("/proc");
	dirent* entry;
	proc_vector* vec = new proc_vector();

	if (!dp) {
		perror("Read /proc directory failed!");
		return NULL;
	}

	char* stat_file_name = new char[255 + 20];
	char* statm_file_name = new char[255 + 20];
	FILE* stat_fp, * statm_fp;

	while ((entry = readdir(dp))) {

		if (!is_number_folder(entry)) continue;

		process* proc = new process();

		sprintf(stat_file_name, "/proc/%s/stat", entry->d_name);
		sprintf(statm_file_name, "/proc/%s/statm", entry->d_name);

		FILE* stat_fp = fopen(stat_file_name, "r");
		if (!stat_fp) {
			delete proc;
			continue;
		}
		fscanf(stat_fp, "%d %s %c", &proc->pid, proc->pname, &proc->state);
		fclose(stat_fp);

		// TODO, Here just read the memory usage in page.
		FILE* statm_fp = fopen(statm_file_name, "r");
		if (!statm_fp) {
			delete proc;
			continue;
		}
		fscanf(statm_fp, "%lu %lu %lu %lu %*u %lu",
			&proc->memory_size,
			&proc->rss_size,
			&proc->shared_size,
			&proc->text_size,
			&proc->data_size);
		fclose(statm_fp);

		proc->thread_vector = get_all_threads(proc->pid);

		vec->push_back(proc);
	}

	closedir(dp);

	delete[] stat_file_name;
	delete[] statm_file_name;

	return vec;
}

/**
* @brief Get all the threads' information about a process
* @par pid, the process id of the process you want to find
* @return thrd_vector*, a pointer to a vector which contains information about all the threads.
*/
thrd_vector* get_all_threads(int pid) {
	char* dir_name = new char[255 + 20];
	sprintf(dir_name, "/proc/%d/task", pid);

	DIR* dp = opendir(dir_name);
	dirent* entry;
	thrd_vector* vec = new thrd_vector();

	if (!dp) return NULL;

	char* stat_file_name = new char[255 + 20];
	char* statm_file_name = new char[255 + 20];
	FILE* stat_fp, * statm_fp;

	while ((entry = readdir(dp))) {

		if (!is_number_folder(entry)) continue;

		thread* thrd = new thread();

		sprintf(stat_file_name, "%s/%s/stat", dir_name, entry->d_name);
		sprintf(statm_file_name, "%s/%s/statm", dir_name, entry->d_name);

		stat_fp = fopen(stat_file_name, "r");
		if (!stat_fp) {
			delete thrd;
			continue;
		}
		fscanf(stat_fp, "%d %*s %c", &thrd->tid, &thrd->state);
		fclose(stat_fp);

		statm_fp = fopen(statm_file_name, "r");
		if (!statm_fp) {
			delete thrd;
			continue;
		}
		fscanf(statm_fp, "%lu %lu %lu %lu %*u %lu",
			&thrd->memory_size,
			&thrd->rss_size,
			&thrd->shared_size,
			&thrd->text_size,
			&thrd->data_size);
		fclose(statm_fp);

		vec->push_back(thrd);

	}

	closedir(dp);
	delete[] dir_name;
	delete[] stat_file_name;
	delete[] statm_file_name;

	return vec;
}

void delete_process_vec(proc_vector* vec) {
	for (proc_vector_iter it = vec->begin(); it != vec->end(); it++) {
		thrd_vector* thread_vector = (*it)->thread_vector;
		if (thread_vector != NULL) {
			for (thrd_vector_iter thrd_it = thread_vector->begin(); thrd_it != thread_vector->end(); ++thrd_it) {
				delete* thrd_it;
			}
			delete (*it)->thread_vector;
		}

		delete[](*it)->pname;
		delete (*it);
	}
	delete vec;
}

memory* get_memory_info() {

	const char* memory_info_directory = "/proc/meminfo";
	ifstream infile;
	infile.open(memory_info_directory);
	char* tmp = new char[256];
	memory* mem = new memory();
	infile >> tmp >> mem->memTotal >> tmp >> tmp >> mem->memFree
		>> tmp >> tmp >> mem->MemAvailable >> tmp >> tmp
		>> mem->Buffers >> tmp >> tmp >> mem->Cached;
	return mem;

}

int cmp_pid(process* a, process* b) {
	return a->pid < b->pid;
}

int cmp_pid_inv(process* a, process* b) {
	return a->pid > b->pid;
}

int cmp_mem(process* a, process* b) {
	return a->memory_size < b->memory_size;
}

int cmp_mem_inv(process* a, process* b) {
	return a->memory_size > b->memory_size;
}

int cmp_name(process* a, process* b) {
	return strcmp(a->pname, b->pname)<0;
}

int cmp_name_inv(process* a, process* b) {
	return strcmp(a->pname, b->pname)>0;
}


void sort_mem_vector(proc_vector* vec, int sort_method) {
	switch (sort_method) {
	case 0:
		sort(vec->begin(), vec->end(), cmp_pid);
		break;
	case 1:
		sort(vec->begin(), vec->end(), cmp_pid_inv);
		break;
	case 2:
		sort(vec->begin(), vec->end(), cmp_mem);
		break;
	case 3:
		sort(vec->begin(), vec->end(), cmp_mem_inv);
		break;
	case 4:
		sort(vec->begin(), vec->end(), cmp_name);
		break;
	case 5:
		sort(vec->begin(), vec->end(), cmp_name_inv);
		break;
	}
}