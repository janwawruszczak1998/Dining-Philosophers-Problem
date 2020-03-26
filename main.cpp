#include <iostream>
#include <vector>
#include <ncurses.h>
#include "Fork.h"
#include "Philosopher.h"

int NUM_OF_PHILOSOPHERS;    // zakladamy tyle samo widelcow
int y_single_window = 4;

// funkcja rysujaca granice okienek filozofow i widelcow
void draw_border(WINDOW *win, char type)
{
    if (type == 'P')
    {
        wattron(win, COLOR_PAIR(1));
        box(win, 0, 0);
        wattroff(win, COLOR_PAIR(1));
    }
    else
    {
        wattron(win, COLOR_PAIR(2));
        box(win, 0, 0);
        wattroff(win, COLOR_PAIR(2));
    }
}

int main() {

    end_flag = false;
    // inicjalizacja widoku ncurses
    initscr();
    echo();
    nocbreak();

    start_color();
    init_pair(1, COLOR_BLUE, COLOR_BLACK);
    init_pair(2, COLOR_GREEN, COLOR_BLACK);

    // uzyskanie maks rozmiaru okna
    int y_max_size, x_max_size;
    getmaxyx(stdscr, y_max_size, x_max_size);

    WINDOW *inputwin = newwin(y_single_window, x_max_size - 10, 0, 5);
    box(inputwin, 0, 0);
    refresh();
    wmove(inputwin, 2, 2);
    wrefresh(inputwin);

    // wczytanie parametru okreslajacego liczbe filozofow/widelcow
    mvwprintw(inputwin, 1, 1, "If you want to end program just type q, press enter and wait a a few seconds. Set the number of philosophers: ");
    wrefresh(inputwin);
    wscanw(inputwin, "%d", &NUM_OF_PHILOSOPHERS);
    wrefresh(inputwin);


    // vectory wskaznikow na okienka kazdego z filozofow
    std::vector<WINDOW *> phil_windows;
    std::vector<WINDOW *> fork_windows;

    for (int i = 0; i < NUM_OF_PHILOSOPHERS; i++)   // wytworzenie okienek
    {
        phil_windows.push_back(newwin(y_single_window, (x_max_size - 10) / 2, (i + 1) * y_single_window, 5));
        fork_windows.push_back(newwin(y_single_window, (x_max_size - 10) / 2, (i + 1) * y_single_window, x_max_size / 2));
        draw_border(phil_windows[i], 'P');
        draw_border(fork_windows[i], 'F');
        wrefresh(phil_windows[i]);
        wrefresh(fork_windows[i]);
    }


    // utworzenie widelcow i filozofow
    std::vector<std::unique_ptr<Philosopher>> philosophers;
    std::vector<std::unique_ptr<Fork>> forks;
    for(int i = 0; i < NUM_OF_PHILOSOPHERS; ++i){
        std::unique_ptr<Fork> fork(new Fork(fork_windows[i]));
        forks.push_back(std::move(fork));
        forks[i]->show_status();
    }
    for(int i = 0; i < NUM_OF_PHILOSOPHERS; ++i){
        std::unique_ptr<Philosopher> philosopher(new Philosopher(i, NUM_OF_PHILOSOPHERS, forks, phil_windows[i]));
        philosophers.push_back(std::move(philosopher));
    }
    wrefresh(inputwin);

    while (true)
    {
        if (wgetch(inputwin) == 'q')
        {
            end_flag = true;
            break;
        }
    }

    // uruchomienie watkow filozofow
    for(int i = 0; i < NUM_OF_PHILOSOPHERS; ++i) {
        philosophers[i]->phi_thread.join();
    }

    endwin(); // dealokuje zasoby z ncurses
    return 0;
}
