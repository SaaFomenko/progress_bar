#include <chrono>
#include <ctime>
#include <iostream>
#include <mutex>
#include <thread>


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

    void test_throw(const int i, const int rnd)
    {
        if (i == rnd)
        {
            throw "Fail event";
        }
    }

    void print_progress(int i, int n)
    {
        srand(time(NULL));
        using Ms = std::chrono::milliseconds;
        const int bar_size = 10;
        int k = 0;
        
        m.lock();
        const char* header = i != 0 ? "" : "N\tid\t\tProgress Bar  Time\n";
        std::cout << header << "\033[" << i + 2 << ";0H" << i << '\t' 
            << std::this_thread::get_id() << "\t";
        m.unlock();

        auto start = std::chrono::steady_clock::now();
        for (int x = 0; x < bar_size; ++x)
        {
            try
            {
                k = rand() % 10 + 1;
                test_throw(x, k);
                std::this_thread::sleep_for(Ms(x + k * 100));
                std::lock_guard<std::mutex> lock_trhl_l(l);
                std::cout << std::unitbuf << "\033[" << i + 2 
                    << ";" << x + 25 << "H\033[42m ";
            }
            //catch(const std::exception& e)
            catch(const char* e)
            {
                //std::cerr << e.what() << '\n';
                k = rand() % 10 + 1;
                std::this_thread::sleep_for(Ms(x + k * 100));
                std::lock_guard<std::mutex> lock_trhl_l(l);
                std::cout << std::unitbuf << "\033[" << i + 2 
                    << ";" << x + 25 << "H\033[41m ";
            }
        }
        auto end = std::chrono::steady_clock::now();
        std::chrono::duration<double> elapsed_seconds = end - start;

        std::lock_guard<std::mutex> lock_trh_m(m);
        std::this_thread::sleep_for(Ms(300));
        //std::cout << def << "\033[" << i + 2 << ";32H"
        std::cout << "\033[49m\033[" << i + 2 << ";38H"
            << elapsed_seconds.count() << "s";
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

    std::cout << "\033[" << n + 2 << ";0H" << '\n';

    return 0;
}
