#include "process.hpp"
#include <cstdio>
#include <ctype.h>
#include <cstdlib>
#include <dirent.h>

/** To check whether a folder is a pid folder.
 *  Return true if it is, else false. This is
 *  a inner function only.
 **/
bool is_number_folder(dirent *entry) {

	const char *p = entry->d_name;
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
	DIR *dp = opendir("/proc");
	dirent *entry;
	proc_vector *vec = new proc_vector();

	if (!dp) {
		perror("Read /proc directory failed!");
		return NULL;
	}

	char *stat_file_name = new char[255 + 20];
	char *statm_file_name = new char[255 + 20];
	FILE *stat_fp, *statm_fp;

	while ((entry = readdir(dp))) {

		if (!is_number_folder(entry)) continue;

		process *proc = new process();
		
		
		sprintf(stat_file_name, "/proc/%s/stat", entry->d_name);
		sprintf(statm_file_name, "/proc/%s/statm", entry->d_name);

		stat_fp = fopen(stat_file_name, "r");
		fscanf(stat_fp, "%d %s %c", &proc->pid, proc->pname, &proc->state);
		fclose(stat_fp);

		// TODO, Here only read the memory usage in page.
		statm_fp = fopen(statm_file_name, "r");
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
	char *dir_name = new char[255+20];
	sprintf(dir_name, "/proc/%d/task", pid);

	DIR *dp = opendir(dir_name);
	dirent *entry;
	thrd_vector *vec = new thrd_vector();

	if (!dp) return NULL;

	char *stat_file_name = new char[255 + 20];
	char *statm_file_name = new char[255 + 20];
	FILE *stat_fp, *statm_fp;

	while((entry = readdir(dp))) {

		if (!is_number_folder(entry)) continue;

		thread *thrd = new thread();

		sprintf(stat_file_name, "%s/%s/stat", dir_name, entry->d_name);
		sprintf(statm_file_name, "%s/%s/statm", dir_name, entry->d_name);

		stat_fp = fopen(stat_file_name, "r");
		printf("%s\n", stat_file_name);
		fscanf(stat_fp, "%d %*s %c", &thrd->tid, &thrd->state);
		fclose(stat_fp);

		statm_fp = fopen(statm_file_name, "r");
		fscanf(statm_fp, "%lu %lu %lu %lu %*u %lu",
					&thrd->memory_size,
					&thrd->rss_size,
					&thrd->shared_size,
					&thrd->text_size,
					&thrd->data_size);
		fclose(statm_fp);

		vec->push_back(thrd);
		
	}
	
	delete[] dir_name;
	delete[] stat_file_name;
	delete[] statm_file_name;

	return vec;
}