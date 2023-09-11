#include <atomic>
#include <chrono>
#include <unistd.h>
#include <iostream>
#include <mutex>
#include <thread>
#include "lib/colormod.h"
#include <cstdlib>


using namespace std::chrono_literals;

void clear_screen()
{
#ifdef WINDOWS
    std::system("cls");
#else
    // Assume POSIX
    std::system ("clear");
#endif
}

namespace my
{
    std::mutex m;
    std::mutex l;
    //std::atomic<int> thr{0};
    //std::atomic<std::thread::id> thr_id;
    //std::atomic<std::chrono::duration<double>> elapsed_seconds;
    //std::atomic<char> Spc{' '};
    //std::atomic<char> Non{0};
    //std::atomic<char> End{'\n'};
    //std::atomic<char> Tab{'\t'};
    void print_to_pos(int x, int y, const char symvol)
    {
        std::cout << "\033[42m\033[" << y + 1 << ';' << x + 1 << 'H';

        std::cout << symvol;
    }

    void print_progress(int i, int n)
    {
        const int bar_size = 20;
        //thr.store(i);
        //thr_id.store(std::this_thread::get_id());
        Color::Modifier green(Color::BG_GREEN);
        Color::Modifier def(Color::BG_DEFAULT);

        std::lock_guard<std::mutex> lock_trh(m);
        //std::unique_lock<std::mutex> lm(m);

        if (i == 0)
        {
            std::cout << "N\tid\t\tProgress Bar\t\tTime\n";
        }

        std::cout << i << '\t' << std::this_thread::get_id() << "\t\n";
        //std::cout << thr.load() << '\t' << thr_id.load() << '\t';

        //lock_trh.~lock_guard();

        // auto start = std::chrono::steady_clock::now();
        // for(int y = 0; y < n; ++y)
        // {
        //     for (int x = 0; x < bar_size; ++x)
        //     {
        //         std::this_thread::sleep_for(0.2s);
        //         std::lock_guard<std::mutex> lock_trh(l);
        //         std::cout.setf(std::ios::unitbuf);
        //         //std::cout << green << ' ';
        //         print_to_pos(x, y, ' ');
        //     }
        // }
        // auto end = std::chrono::steady_clock::now();
        // std::chrono::duration<double> elapsed_seconds = end - start;

        // std::cout << def << '\t' << elapsed_seconds.count() << "s\n";
    }
}

int main()
{
    clear_screen();
    std::ios_base::sync_with_stdio(false);

    int n = 5;

    std::thread* thr_arr = new std::thread[n];

    for (int i = 0; i < n; ++i)
    {
        thr_arr[i] = std::thread(my::print_progress, i, n);
    }

    for (int i = 0; i < n; ++i)
    {
        if (thr_arr[i].joinable())
            thr_arr[i].join();
    }

    return 0;
}