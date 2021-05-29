#include "process.hpp"
#include <ncurses.h>
#include <unistd.h>
#include <string>
#include <time.h>

#define ESCAPE 27
#define ENTER 10

proc_vector* vec;
int begin = 0, sort_method = 0, choose = 0;
clock_t starttime;
memory* mem;

void show_win() {
    attroff(A_REVERSE);
    mvprintw(0, 0, "KB Mem: %u total, %u free, %u available, %u buffer, %u cached\n",
        mem->memTotal,
        mem->memFree,
        mem->MemAvailable,
        mem->Buffers,
        mem->Cached);
    // attron(A_REVERSE);
    mvprintw(1, 0, "PID\tSTATUS\tMEMORY\tRSS\tSHARED\tTEXT\tDATA\tNAME\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\n");
    attron(A_REVERSE);
    switch (choose) {
    case 0:
        mvprintw(1, 0, "PID");
        break;
    case 1:
        mvprintw(1, 16, "MEMORY");
        break;
    case 2:
        mvprintw(1, 56, "NAME");
        break;
    }
    attroff(A_REVERSE);
    // attroff(A_REVERSE);
    int maxy, maxx;
    getmaxyx(stdscr, maxy, maxx);
    if (begin + maxy - 1 >= vec->size()) {
        begin = vec->size() - maxy;
    }
    int cnt = 2;
    for (int i = 0; i < vec->size(); i++)
    {
        process* it = vec->at((i + begin) % vec->size());


        mvprintw(cnt++, 0, "%u\t%c\t%d\t%d\t%d\t%d\t%d\t%s\n",
            it->pid, it->state,
            it->memory_size, it->rss_size,
            it->shared_size, it->text_size,
            it->data_size, it->pname);
        if (it->thread_vector->size() != 1
            || it->thread_vector->at(0)->tid != it->pid && cnt < maxy - 1) {
            thrd_vector* thread_vector = it->thread_vector;
            mvprintw(cnt - 1, 90, "-> with %d sub threads.\n", thread_vector->size());

        }

        if (cnt >= maxy - 1) {
            break;
        }
    }
    attron(A_REVERSE);
    mvprintw(maxy - 1, 0, "p: exit\tup/down: scroll the information\t\tleft/right+Enter: sort method");
    if ((clock() - starttime) / CLOCKS_PER_SEC >= 1) {
        delete_process_vec(vec);
        delete mem;
        vec = get_all_processes();
        sort_mem_vector(vec, sort_method);
        starttime = clock();
        mem = get_memory_info();
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
        if (key == KEY_DOWN)
            begin++;

        if (key == KEY_UP && begin>0) {
            begin--;
        }
        if (key == ESCAPE || key == 'q')
            break;

        if (key == KEY_LEFT && choose > 0) {
            choose=(choose-1)%3;
        }
        if (key == KEY_RIGHT && choose < 2) {
            choose=(choose+1)%3;
        }
        if (key == ENTER) {
            if (sort_method / 2 != choose)
                sort_method = choose * 2;
            else {
                if (sort_method % 2 == 0) {
                    sort_method++;
                }
                else {
                    sort_method--;
                }
            }
        }

    }
    delete_process_vec(vec);
    delete mem;
    endwin();
}
