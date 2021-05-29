#include <cstdio>
#include "process.hpp"

int main() {
    memory* mem = get_memory_info();
    printf("%u\t%u\t%u\t%u\t%u\n",
        mem->memTotal,
        mem->memFree,
        mem->MemAvailable,
        mem->Buffers,
        mem->Cached);
    delete(mem);
}