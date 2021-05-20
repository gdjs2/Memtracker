#include <cstdio>
#include <pthread.h>

pthread_t threads[5];

void *function(void* x) {
	unsigned long size = (unsigned long)x;
	char *buffer = new char[size];

	while(1);
}

int main() {
	for (int i = 0; i < 5; ++i)
		pthread_create(&threads[i], NULL, function, (void*)(unsigned long)(4096*i));
	for (int i = 0; i < 5; ++i) {
		pthread_join(threads[i], NULL);
	}
	
	return 0;
}