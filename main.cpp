#include <atomic>
#include <chrono>
#include <cstdlib>
#include <unistd.h>
#include <iostream>
#include <mutex>
#include <thread>
#include "lib/colormod.h"
#include "lib/big_integer.h"


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
    big_integer result("0");
    big_integer num1("999991239");
    big_integer num2("99993");
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

    void set_cursor_pos(int x, int y)
    {
        std::cout << "\033[" << y + 1 << ';' << x + 1 << 'H'; 
    }

    void print_progress(int i, int n)
    {
        using Ms = std::chrono::milliseconds;
        const int bar_size = 10;
        //thr.store(i);
        //thr_id.store(std::this_thread::get_id());
        Color::Modifier green(Color::BG_GREEN);
        Color::Modifier def(Color::BG_DEFAULT);

        //std::unique_lock<std::mutex> lm(m);

        if (i == 0)
        {
            std::cout << "N\tid\t\tProgress Bar\tTime\n";
        }

        //std::cout << thr.load() << '\t' << thr_id.load() << '\t';
        set_cursor_pos(0, i + 1);
        std::cout << i << '\t' << std::this_thread::get_id() << "\t";

        //lock_trh.~lock_guard();

        auto start = std::chrono::steady_clock::now();
        // for(int y = 0; y < n; ++y)
        // {
            for (int x = 0; x < bar_size; ++x)
            {
                //std::this_thread::sleep_for(Ms(x + 1 * 1000));
                std::this_thread::sleep_for(0.9s);
                std::lock_guard<std::mutex> lock_trhl_l(l);
                result = num1 * num2;
                result = result + result;
                //std::cout.setf(std::ios::unitbuf);
                std::cout << std::unitbuf << "\033[42m\033[" << i + 2 
                    << ";" << x + 25 << "H";
                //print_to_pos(x + 24, i + 1, ' ');
                //std::this_thread::sleep_for(Ms(x + 1 * 100));
                //std::this_thread::sleep_for(0.9s);
                //set_cursor_pos(x + 24, i + 1);
                std::cout << green << ' ';
            }
//        }
        auto end = std::chrono::steady_clock::now();
        std::chrono::duration<double> elapsed_seconds = end - start;

        std::lock_guard<std::mutex> lock_trh_m(m);
        //std::cout << std::nounitbuf;
        //set_cursor_pos(40, i + 1);
        //std::cout << def << elapsed_seconds.count() << "s";
       // std::cout << def;
        //std::cout << def << "\033[" << i + 1 << ";40H"; 
        //std::cout << std::nounitbuf << def << "\033[" << i + 2 << ";40H"
        std::this_thread::sleep_for(0.3s);
        std::cout << def << "\033[" << i + 2 << ";40H"
            << elapsed_seconds.count() << "s";

        //std::cout << std::nounitbuf << "\033[" << i + (n - i) + 2 << ";0H" << '\n';
        //std::cout << def << elapsed_seconds.count() << "s\n";
        //std::lock_guard<std::mutex> lock_trh_m(m);
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

    std::cout << "\033[" << n + 3 << ";0H" << my::result << '\n';

    return 0;
}