#include <iostream>
#include <array>
#include <chrono>
#include <sdsl/bit_vectors.hpp>
#include "sparse_array.hpp"

using namespace std;
using namespace sdsl;
using namespace std::chrono;

// g++ -std=c++11 -O3 -DNDEBUG -I ~/include -L ~/lib part3.cpp -o part3 rank_support.cpp select_support.cpp sparse_array.cpp -lsdsl -ldivsufsort -ldivsufsort64

int main()
{

    int n_sizes = 4;
    int sizes[n_sizes] = {1000, 10000, 100000, 1000000};

    int n_sparsities = 3;
    double sparsities[n_sparsities] = {.01, .05, .1};

    int n_repeats = 10;
    
    double get_at_rank_times[n_repeats][n_sizes][n_sparsities];
    double get_at_index_times[n_repeats][n_sizes][n_sparsities];
    double num_elem_at_times[n_repeats][n_sizes][n_sparsities];

    for (int k=0; k<n_repeats; k++) {
        cout << "Repeat " << k << endl;

        high_resolution_clock::time_point start_time;
        high_resolution_clock::time_point end_time;
        duration<double, std::milli> diff_time;

        for (int j=0; j<n_sparsities; j++) {
            int s = sparsities[j];
            cout << s << endl;
            for (int i = 0; i < n_sizes; i++) {
                uint64_t n = sizes[i];
                cout << n << endl;

                SparseArray* sa;

                sa->create(16);

                cout << "fill array" << endl;
                for (int x=0; x<ceil(n*s); x++) {
                    string elem = to_string(x);
                    int r = rand() % n;
                    bool success = sa->append(elem, r);
                    while (!success) {
                        r = rand() % n;
                        success = sa->append(elem, r);
                    }
                }
                cout << "get at rank" << endl;
                start_time = high_resolution_clock::now();
                for (int j = 0; j < 100; j++) {
                    int r = rand() % (int)ceil(n*s);
                    string elem;
                    sa->get_at_rank(r, elem);
                }
                end_time = high_resolution_clock::now();
                diff_time = (end_time - start_time);
                get_at_rank_times[k][i][j] = diff_time.count();

                cout << "get at index" << endl;
                start_time = high_resolution_clock::now();
                for (int j = 0; j < 100; j++) {
                    int r = rand() % n;
                    string elem;
                    sa->get_at_index(r, elem);
                }
                end_time = high_resolution_clock::now();
                diff_time = (end_time - start_time);
                get_at_index_times[k][i][j] = diff_time.count();

                cout << "num elem at" << endl;
                start_time = high_resolution_clock::now();
                for (int j = 0; j < 100; j++) {
                    int r = rand() % n;
                    sa->num_elem_at(r);
                }
                end_time = high_resolution_clock::now();
                diff_time = (end_time - start_time);
                num_elem_at_times[k][i][j] = diff_time.count();
            }
        }
    }

    // ofstream file_obj;
    // file_obj.open("data/part3_creation_times.csv");
    // file_obj << sizes[0];
    // for(int i=1; i < n_sizes; i++) {
    //     file_obj << "," << sizes[i];
    // }
    // file_obj << endl;
    // for(int k=0; k < n_repeats; k++) {
    //     file_obj << creation_times[k][0];
    //     for(int i=1; i < n_sizes; i++) {
    //         file_obj << "," << creation_times[k][i];
    //     }    
    //     file_obj << endl;
    // }
    // file_obj.close();

    // file_obj.open("data/part3_query_times.csv");
    // file_obj << sizes[0];
    // for(int i=1; i < n_sizes; i++) {
    //     file_obj << "," << sizes[i];
    // }
    // file_obj << endl;
    // for(int k=0; k < n_repeats; k++) {
    //     file_obj << query_times[k][0];
    //     for(int i=1; i < n_sizes; i++) {
    //         file_obj << "," << query_times[k][i];
    //     }    
    //     file_obj << endl;
    // }
    // file_obj.close();
}

// Timing instructions from: https://www.apress.com/gp/blog/all-blog-posts/timing-things-in-c-plus-plus/17405398#:~:text=If%20you%20want%20to%20measure,You%20gotta%20start%20somewhere.