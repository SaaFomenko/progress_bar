#include <chrono>
#include <ctime>
#include <iostream>
#include <mutex>
#include <thread>
#include <tuple>
#include <vector>


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

        std::vector<std::tuple<const char*, int>> cols{
            {"N", 3},
            {"id", 16},
            {"Progress Bar", 13},
            {"Time", 10}
        };
        using Ms = std::chrono::milliseconds;
        const int bar_size = 10;
        int k = 0;
        int row_num = i + 2;
        int col_num = std::get<int>(cols.at(0)) + 1; 
        
        m.lock();
        //const char* header = i != 0 ? "" : "N\tid\t\tProgress Bar  Time\n";
        if (i == 0)
        {
            int j = 0;
            for (std::tuple<const char*, int> col : cols)
            {
                std::cout  << "\033[0;" << j << 'H' << std::get<const char*>(col);
                j += std::get<int>(col) + 1;
            }
        }
        std::cout << "\033[" << row_num << ";0H" << i << "\033[" << row_num << ';'
            << col_num << 'H' << std::this_thread::get_id();
        m.unlock();

        col_num += std::get<int>(cols.at(1)) + 1;

        auto start = std::chrono::steady_clock::now();
        for (int x = 0; x < bar_size; ++x)
        {
            int col_x = col_num + x;
            try
            {
                k = rand() % 10 + 1;
                test_throw(x, k);
                std::this_thread::sleep_for(Ms(x + k * 100));
                std::lock_guard<std::mutex> lock_trhl_l(l);
                std::cout << std::unitbuf << "\033[" << row_num
                    << ';' << col_x << "H\033[42m ";
            }
            //catch(const std::exception& e)
            catch(const char* e)
            {
                //std::cerr << e.what() << '\n';
                k = rand() % 10 + 1;
                std::this_thread::sleep_for(Ms(x + k * 100));
                std::lock_guard<std::mutex> lock_trhl_l(l);
                std::cout << std::unitbuf << "\033[" << row_num
                    << ';' << col_x << "H\033[41m ";
            }
        }
        auto end = std::chrono::steady_clock::now();
        std::chrono::duration<double> elapsed_seconds = end - start;

        col_num += std::get<int>(cols.at(2)) + 1;

        std::lock_guard<std::mutex> lock_trh_m(m);
        std::this_thread::sleep_for(Ms(300));
        //std::cout << def << "\033[" << i + 2 << ";32H"
        std::cout << "\033[49m\033[" << row_num << ';' << col_num << 'H'
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
