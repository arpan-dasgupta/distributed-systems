/* MPI Program Template */

#include <stdio.h>
#include <string.h>
#include "mpi.h"
#include <fstream>
#include <bits/stdc++.h>
using namespace std;
typedef long long int ll;
int numpr;
#define send_data_tag 2001
#define return_data_tag 2002

int partition(vector<int> &arr, int l, int r)
{
    int pvt = l + rand() % (r - l + 1);
    // for (ll i = l; i <= r; i++)
    // {
    //     cout << arr[i] << " ";
    // }
    // cout << '\n';
    swap(arr[pvt], arr[r]);
    int fe = l;
    for (int i = l; i < r; i++)
    {
        if (arr[i] < arr[r])
        {
            swap(arr[i], arr[fe]);
            fe++;
        }
    }
    // for (ll i = l; i <= r; i++)
    // {
    //     cout << arr[i] << " ";
    // }
    // cout << '\n';
    swap(arr[r], arr[fe]);
    // for (ll i = l; i <= r; i++)
    // {
    //     cout << arr[i] << " ";
    // }
    // cout << '\n';
    return fe;
}

void send_vec(vector<int> &arr, int rank)
{
    int val = arr.size();
    MPI_Send(&val, 1, MPI_INT, rank, send_data_tag, MPI_COMM_WORLD);
    MPI_Send(&arr[0], val, MPI_INT, rank, send_data_tag, MPI_COMM_WORLD);
}

void receive_vec(vector<int> &arr, int rank)
{
    int num;
    MPI_Status stat;
    MPI_Recv(&num, 1, MPI_INT, rank, send_data_tag, MPI_COMM_WORLD, &stat);
    arr.resize(num);
    MPI_Recv(&arr[0], num, MPI_INT, rank, send_data_tag, MPI_COMM_WORLD, &stat);
}

void quicksort(vector<int> &arr, int low, int high)
{
    // cout << low << " " << high << '\n';
    if (low < high)
    {
        int pos = partition(arr, low, high);
        quicksort(arr, low, pos - 1);
        quicksort(arr, pos + 1, high);
    }
}

void quicksort_parallel(vector<int> &arr, int low, int high, int depth, int rank)
{
    if (low < high)
    {
        int pos = partition(arr, low, high);

        int next_rank = rank + pow(2, depth);
        depth++;
        if (next_rank < numpr)
        {
            send_vec(arr, next_rank);
            int st = pos - 1;
            MPI_Send(&depth, 1, MPI_INT, next_rank, send_data_tag, MPI_COMM_WORLD);
            MPI_Send(&low, 1, MPI_INT, next_rank, send_data_tag, MPI_COMM_WORLD);
            MPI_Send(&st, 1, MPI_INT, next_rank, send_data_tag, MPI_COMM_WORLD);

            quicksort_parallel(arr, pos + 1, high, depth, rank);

            vector<int> new_vec;
            receive_vec(new_vec, next_rank);
            for (int i = low; i < pos - 1; i++)
            {
                arr[i] = new_vec[i];
            }
        }
        else
        {
            quicksort(arr, low, pos - 1);
            quicksort(arr, pos + 1, high);
        }
    }
}

int main(int argc, char **argv)
{

    int rank, numprocs;

    /* start up MPI */
    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &numprocs);

    /*synchronize all processes*/
    MPI_Barrier(MPI_COMM_WORLD);
    double tbeg = MPI_Wtime();
    // vector<pair<int,int>> trans

    int n;
    numpr = numprocs;
    vector<int> arr;
    if (rank == 0)
    {
        ifstream cin(argv[1]);
        cin >> n;
        arr.resize(n);
        for (int i = 0; i < n; i++)
        {
            cin >> arr[i];
        }
        vector<int> final(arr.begin(), arr.end());
        quicksort_parallel(final, 0, n - 1, 0, 0);

        // sort(arr.begin(), arr.end());
        // assert(arr == final);

        ofstream cout(argv[2]);
        for (int i = 0; i < final.size(); i++)
        {
            cout << final[i] << " ";
        }
    }
    else
    {
        vector<int> arr;

        int rr = 1;
        while (rr <= rank)
        {
            rr *= 2;
        }
        rr /= 2;
        int sender = rank - rr;

        MPI_Status stat;
        receive_vec(arr, sender);
        int l, r, dep;
        MPI_Recv(&l, 1, MPI_INT, sender, send_data_tag, MPI_COMM_WORLD, &stat);
        MPI_Recv(&r, 1, MPI_INT, sender, send_data_tag, MPI_COMM_WORLD, &stat);
        MPI_Recv(&dep, 1, MPI_INT, sender, send_data_tag, MPI_COMM_WORLD, &stat);

        quicksort_parallel(arr, l, r, dep, rank);

        send_vec(arr, sender);
    }

    // MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);

    MPI_Barrier(MPI_COMM_WORLD);
    double elapsedTime = MPI_Wtime() - tbeg;
    double maxTime;
    MPI_Reduce(&elapsedTime, &maxTime, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
    if (rank == 0)
    {
        printf("Total time (s): %f\n", maxTime);
    }

    /* shut down MPI */
    MPI_Finalize();
    return 0;
}