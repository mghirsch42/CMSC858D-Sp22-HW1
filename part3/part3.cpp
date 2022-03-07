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
    
    double create_and_fill_times[n_repeats][n_sizes][n_sparsities];
    double get_at_rank_times[n_repeats][n_sizes][n_sparsities];
    double get_at_index_times[n_repeats][n_sizes][n_sparsities];
    double num_elem_at_times[n_repeats][n_sizes][n_sparsities];

    for (int k=0; k<n_repeats; k++) {
        cout << "Repeat " << k << endl;

        high_resolution_clock::time_point start_time;
        high_resolution_clock::time_point end_time;
        duration<double, std::milli> diff_time;

        for (int j=0; j<n_sparsities; j++) {
            double s = sparsities[j];
            cout << s << endl;
            for (int i = 0; i < n_sizes; i++) {
                uint64_t n = sizes[i];
                cout << n << endl;

                SparseArray* sa = new SparseArray;

                start_time = high_resolution_clock::now();
                sa->create(n);
                // cout << "fill array" << endl;
                for (int x=0; x<ceil(n*s); x++) {
                    string elem = to_string(x);
                    int r = rand() % n;
                    bool success = sa->append(elem, r);
                    while (!success) {
                        r = rand() % n;
                        success = sa->append(elem, r);
                    }
                }
                sa->build_rank_support();
                end_time = high_resolution_clock::now();
                diff_time = (end_time - start_time);
                create_and_fill_times[k][i][j] = diff_time.count();

                // cout << "get at rank" << endl;
                start_time = high_resolution_clock::now();
                for (int j = 0; j < 100; j++) {
                    int r = rand() % (int)(n*s);
                    string elem;
                    sa->get_at_rank(r, elem);
                }
                end_time = high_resolution_clock::now();
                diff_time = (end_time - start_time);
                get_at_rank_times[k][i][j] = diff_time.count();

                // cout << "get at index" << endl;
                start_time = high_resolution_clock::now();
                for (int j = 0; j < 100; j++) {
                    int r = rand() % n;
                    string elem;
                    sa->get_at_index(r, elem);
                }
                end_time = high_resolution_clock::now();
                diff_time = (end_time - start_time);
                get_at_index_times[k][i][j] = diff_time.count();

                // cout << "num elem at" << endl;
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

    cout << "Writing files" << endl;

    ofstream file_creation;
    ofstream file_rank;
    ofstream file_index;
    ofstream file_num_elem;

    file_creation.open("part3/results/part3_creation_times.csv");
    file_rank.open("part3/results/part3_get_at_rank_times.csv");
    file_index.open("part3/results/part3_get_at_index_times.csv");
    file_num_elem.open("part3/results/part3_num_elem_at_times.csv");

    file_creation << "sparsity," << sizes[0];
    file_rank << "sparsity," << sizes[0];
    file_index << "sparsity," << sizes[0];
    file_num_elem << "sparsity," << sizes[0];

    for(int i=1; i < n_sizes; i++) {
        file_creation << "," << sizes[i];
        file_rank << "," << sizes[i];
        file_index << "," << sizes[i];
        file_num_elem << "," << sizes[i];

    }
    file_creation << endl;
    file_rank << endl;
    file_index << endl;
    file_num_elem << endl;

    for(int s=0; s<n_sparsities; s++) {
        file_creation << sparsities[s];
        file_rank << sparsities[s];
        file_index << sparsities[s];
        file_num_elem << sparsities[s];

        for(int k=0; k < n_repeats; k++) {
            for(int i=0; i < n_sizes; i++) {
                file_creation << "," << create_and_fill_times[k][i][s];
                file_rank << "," << get_at_rank_times[k][i][s];
                file_index << "," << get_at_index_times[k][i][s];
                file_num_elem << "," << num_elem_at_times[k][i][s];

            }    
            file_creation << endl;
            file_rank << endl;
            file_index << endl;
            file_num_elem << endl;
        }
    }
    file_creation.close();
    file_rank.close();
    file_index.close();
    file_num_elem.close();
}

// Timing instructions from: https://www.apress.com/gp/blog/all-blog-posts/timing-things-in-c-plus-plus/17405398#:~:text=If%20you%20want%20to%20measure,You%20gotta%20start%20somewhere.