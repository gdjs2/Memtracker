#include "process.hpp"
#include <ncurses.h>
#include <unistd.h>
#include <string>
#include <time.h>

#define ESCAPE 27

proc_vector* vec;
int begin = 0;
clock_t starttime;
memory* mem;

void show_win() {
    mvprintw(0,0,"KB Mem: %u total, %u free, %u available, %u buffer, %u cached\n",
        mem->memTotal,
        mem->memFree,
        mem->MemAvailable,
        mem->Buffers,
        mem->Cached);
    attron(A_REVERSE);
    mvprintw(1, 0, "PID\tSTATUS\tMEMORY\tRSS\tSHARED\tTEXT\tDATA\tNAME\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\n");
    attroff(A_REVERSE);
    int maxy, maxx;
    getmaxyx(stdscr, maxy, maxx);
    if (begin + maxy - 1 >= vec->size()) {
        begin = vec->size() - maxy;
    }
    int cnt = 2;
    for (int i = 0; i < vec->size(); i++)
    {
        process* it = vec->at((i + begin) % vec->size());

        mvprintw(cnt++, 0, "%u\t%c\t%d\t%d\t%d\t%d\t%d\t%s",
            it->pid, it->state,
            it->memory_size, it->rss_size,
            it->shared_size, it->text_size,
            it->data_size, it->pname);
    }
    if ((clock() - starttime) / CLOCKS_PER_SEC >= 1) {
        for (proc_vector_iter it = vec->begin(); it != vec->end(); ++it) {
            delete((*it));
        }
        delete(vec);
        vec = get_all_processes();
        starttime = clock();
        // mvprintw(70, 0, "%d", starttime);
        mem=get_memory_info();
    }
    refresh();
}

int main() {
    initscr();
    noecho();
    int key = 0;
    vec = get_all_processes();
    mem = get_memory_info();
    keypad(stdscr, true);
    nodelay(stdscr, true);
    starttime = clock();
    while (true)
    {
        show_win();
        int key = getch();
        if (key == KEY_DOWN) {
            begin++;
        }
        if (key == KEY_UP) {
            begin--;
            if (begin < 0)
                begin = 0;
        }
        mvprintw(0, 50, "%d", vec->size());

    }

    endwin();
}
