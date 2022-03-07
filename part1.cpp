#include <iostream>
#include <array>
#include <chrono>
#include <sdsl/bit_vectors.hpp>
#include "rank_support.hpp"

using namespace std;
using namespace sdsl;
using namespace std::chrono;

// g++ -std=c++11 -O3 -DNDEBUG -I ~/include -L ~/lib part1.cpp -o part1 rank_support.cpp -lsdsl -ldivsufsort -ldivsufsort64

int main()
{
    int n_sizes = 4;
    int sizes[n_sizes] = {1000, 10000, 100000, 1000000};

    int n_repeats = 10;
    
    int overheads[n_repeats][n_sizes];
    double creation_times[n_repeats][n_sizes];
    double query_times[n_repeats][n_sizes];

    for (int k=0; k<10; k++) {
        cout << "Repeat " << k << endl;

        high_resolution_clock::time_point start_time;
        high_resolution_clock::time_point end_time;
        duration<double, std::milli> diff_time;

        for (int i = 0; i < n_sizes; i++) {
            int n = sizes[i];
            // cout << n << endl;

            bit_vector bv(n, 1);
            
            start_time = high_resolution_clock::now();
            RankSupport* rs = new RankSupport(&bv);
            end_time = high_resolution_clock::now();
            diff_time = (end_time - start_time);
            // cout << diff_time.count() << endl;
            creation_times[k][i] = diff_time.count();

            start_time = high_resolution_clock::now();
            for (int j = 0; j < 16; j++) {
                int idx = rand() % n;
                rs->rank1(j);
            }
            end_time = high_resolution_clock::now();
            diff_time = (end_time - start_time);
            // cout << diff_time.count() << endl;
            query_times[k][i] = diff_time.count();

            overheads[k][i] = rs->overhead();
        }
    }

    ofstream file_obj;
    file_obj.open("data/part1_creation_times.csv");
    file_obj << sizes[0];
    for(int i=1; i < n_sizes; i++) {
        file_obj << "," << sizes[i];
    }
    file_obj << endl;
    for(int k=0; k < n_repeats; k++) {
        file_obj << creation_times[k][0];
        for(int i=1; i < n_sizes; i++) {
            file_obj << "," << creation_times[k][i];
        }    
        file_obj << endl;
    }
    file_obj.close();

    file_obj.open("data/part1_query_times.csv");
    file_obj << sizes[0];
    for(int i=1; i < n_sizes; i++) {
        file_obj << "," << sizes[i];
    }
    file_obj << endl;
    for(int k=0; k < n_repeats; k++) {
        file_obj << query_times[k][0];
        for(int i=1; i < n_sizes; i++) {
            file_obj << "," << query_times[k][i];
        }    
        file_obj << endl;
    }
    file_obj.close();

    file_obj.open("data/part1_overheads.csv");
    file_obj << sizes[0];
    for(int i=1; i < n_sizes; i++) {
        file_obj << "," << sizes[i];
    }
    file_obj << endl;
    for(int k=0; k < n_repeats; k++) {
        file_obj << overheads[k][0];
        for(int i=1; i < n_sizes; i++) {
            file_obj << "," << overheads[k][i];
        }    
        file_obj << endl;
    }
    file_obj.close();

}

// Timing instructions from: https://www.apress.com/gp/blog/all-blog-posts/timing-things-in-c-plus-plus/17405398#:~:text=If%20you%20want%20to%20measure,You%20gotta%20start%20somewhere.