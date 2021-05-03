#include "process.hpp"
#include <cstdio>
#include <ctype.h>
#include <cstdlib>

/** To check whether a folder is a pid folder.
 *  Return true if it is, else false. This is
 *  a inner function only.
 **/
bool is_pid_folder(dirent *entry) {

	const char *p = entry->d_name;
	while (*p) {
		if (!isdigit(*p)) return 0;
		++p;
	}
	return 1;
}


proc_vector* get_all_processes() {
	DIR *dp = opendir("/proc");
	dirent *entry;
	proc_vector *vec = new proc_vector();

	if (!dp) {
		perror("Read /proc directory failed!");
		return NULL;
	}

	while (entry = readdir(dp)) {

		if (!is_pid_folder(entry)) continue;

		process *proc = new process();
		
		char *stat_file_name = new char[255 + 20];
		char *statm_file_name = new char[255 + 20];
		sprintf(stat_file_name, "/proc/%s/stat", entry->d_name);
		sprintf(statm_file_name, "/proc/%s/statm", entry->d_name);

		FILE *stat_fp = fopen(stat_file_name, "r");
		fscanf(stat_fp, "%d %s %c", &proc->pid, proc->pname, &proc->state);
		fclose(stat_fp);

		// TODO, Here just read the memory usage in page.
		FILE *statm_fp = fopen(statm_file_name, "r");
		fscanf(statm_fp, "%lu %lu %lu %lu %*u %lu",
					&proc->memory_size,
					&proc->rss_size,
					&proc->shared_size,
					&proc->text_size,
					&proc->data_size);
		fclose(statm_fp);

		vec->push_back(proc);
	}

	return vec;
}