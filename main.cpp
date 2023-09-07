#include <atomic>
#include <chrono>
#include <unistd.h>
#include <iostream>
#include <mutex>
#include <thread>
#include "lib/colormod.h"

using namespace std::chrono_literals;

namespace my
{
    std::mutex m;
    //std::mutex l;
    //std::atomic<int> thr{0};
    //std::atomic<std::thread::id> thr_id;
    //std::atomic<std::chrono::duration<double>> elapsed_seconds;
    std::atomic<char> Spc{' '};
    //std::atomic<char> Non{0};
    //std::atomic<char> End{'\n'};
    //std::atomic<char> Tab{'\t'};

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

        std::cout << i << '\t' << std::this_thread::get_id() << '\t';
        //std::cout << thr.load() << '\t' << thr_id.load() << '\t';

        //lock_trh.~lock_guard();

        auto start = std::chrono::steady_clock::now();
        //for(int i = 0; i < n; ++i)
        //{
            for (int j = 0; j < bar_size; ++j)
            {
                //p.store(' ', std::memory_order_relaxed);
                //std::lock_guard<std::mutex> lock_trh(l);
                std::cout.setf(std::ios::unitbuf);
                std::cout << green << Spc.load(std::memory_order_relaxed);
                //std::cout << green << ' ';
                //sleep(100);
                std::this_thread::sleep_for(0.2s);
            }
        //}
        auto end = std::chrono::steady_clock::now();
        std::chrono::duration<double> elapsed_seconds = end - start;

        std::cout << def << '\t' << elapsed_seconds.count() << "s\n";
        //std::lock(lm, ll);
    }
}

int main()
{
    std::ios_base::sync_with_stdio(false);
    int n = 5;
    // int i = 0;

    //my::print_progress(i, n);

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