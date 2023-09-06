#include <atomic>
#include <chrono>
#include <iostream>
#include <mutex>
#include <thread>
#include "lib/colormod.h"

using namespace std::chrono_literals;

namespace my
{
    std::mutex m;
    std::atomic<char> p{0};

    void print_progress(int i, int n)
    {
        const int bar_size = 20;
        Color::Modifier green(Color::BG_GREEN);
        Color::Modifier def(Color::BG_DEFAULT);

        //std::lock_guard<std::mutex> lock_trh(m);

        m.lock();

        if (i == 0)
        {
            std::cout << "N\tid\t\tProgress Bar\t\tTime\n";
        }


        std::cout << i << '\t' << std::this_thread::get_id() << '\t';
            
        m.unlock();

        auto start = std::chrono::steady_clock::now();
        for(int i = 0; i < bar_size; ++i)
        {
            p.store(' ');
            m.lock();
            std::cout << green << p.load();
            m.unlock();
            std::this_thread::sleep_for(0.3s);
        }
        auto end = std::chrono::steady_clock::now();
        std::chrono::duration<double> elapsed_seconds = end - start;
        m.lock();
        std::cout << def << '\t' << elapsed_seconds.count() << "s\n";
        m.unlock();
    }
}

int main()
{
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