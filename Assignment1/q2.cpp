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

void merge(vector<vector<int>> &all, vector<int> &arr)
{
    int num = all.size();
    multiset<pair<int, int>> mst;
    vector<int> indices(num, 0);
    for (int i = 0; i < num; i++)
    {
        mst.insert({all[i][0], i});
        indices[i]++;
    }
    while (mst.size() > 0)
    {
        auto a = *mst.begin();
        arr.push_back(a.first);
        mst.erase(mst.begin());
        if (indices[a.second] < all[a.second].size())
        {
            mst.insert({all[a.second][indices[a.second]], a.second});
            indices[a.second]++;
        }
    }
}

int main(int argc, char **argv)
{

    freopen(argv[1], "r", stdin);
    freopen(argv[2], "a", stdout);

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
    vector<int> arr;
    if (rank == 0)
    {
        cin >> n;
        arr.resize(n);
        for (int i = 0; i < n; i++)
        {
            cin >> arr[i];
        }
        // quicksort(arr, 0, n - 1);
        // for (ll i = 0; i < n; i++)
        // {
        //     cout << arr[i] << " ";
        // }
        vector<int> start_points(numprocs);
        int jumps = (n + numprocs - 1) / numprocs;
        for (int i = 0; i < numprocs; i++)
        {
            start_points[i] = i * jumps;
        }
        vector<vector<int>> all_vecs;
        vector<int> my_vec;
        for (int i = 0; i < numprocs; i++)
        {
            vector<int> nv;
            for (int j = start_points[i]; j < min(start_points[i] + jumps, n); j++)
            {
                nv.push_back(arr[j]);
            }
            if (i == 0)
            {
                // quicksort(nv, 0, int(nv.size()) - 1);
                my_vec = vector<int>(nv);
                continue;
            }
            int val = nv.size();
            // for (int i = 0; i < val; i++)
            // {
            //     cout << nv[i] << " ";
            // }
            // cout << " ++ \n";
            send_vec(nv, i);
            // vector<int> sv;
            // receive_vec(sv, i);
        }
        quicksort(my_vec, 0, int(my_vec.size()) - 1);
        for (int i = 0; i < numprocs; i++)
        {
            if (i == 0)
                all_vecs.push_back(my_vec);
            else
            {
                vector<int> sv;
                receive_vec(sv, i);
                if (sv.size() > 0)
                    all_vecs.push_back(sv);
            }
        }
        // for (int i = 0; i < all_vecs.size(); i++)
        // {
        //     for (int j = 0; j < all_vecs[i].size(); j++)
        //     {
        //         cout << all_vecs[i][j] << " ";
        //     }
        //     cout << '\n';
        // }
        vector<int> final;
        merge(all_vecs, final);
        for (int i = 0; i < final.size(); i++)
        {
            cout << final[i] << " ";
        }
    }
    else
    {
        vector<int> arr;
        // MPI_Send(&nv, nv.size(), MPI_INT, i, 0, MPI_COMM_WORLD);
        // cout << num << '\n';
        receive_vec(arr, 0);
        int num = arr.size();
        quicksort(arr, 0, num - 1);
        // for (int i = 0; i < num; i++)
        // {
        //     cout << arr[i] << " ";
        // }
        // cout << '\n';
        send_vec(arr, 0);
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