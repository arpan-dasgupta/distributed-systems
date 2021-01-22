/* MPI Program Template */

#include <stdio.h>
#include <string.h>
#include "mpi.h"
#include <fstream>
#include <bits/stdc++.h>
using namespace std;
typedef long long int ll;
#define send_data_tag 2001
#define return_data_tag 2002

int nump;

typedef struct graph_data
{
    vector<vector<int>> adj;
    vector<int> colors;
    int l, r;
} gd;

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

void color_reduce(vector<int> &color, int l, int r, vector<vector<int>> &adj, int del)
{
    for (int i = del + l + 1; i <= r; i++)
    {
        int mx = l;
        set<int> used;
        for (auto vert : adj[i])
        {
            used.insert(color[vert]);
        }
        while (used.find(mx) != used.end())
        {
            mx++;
        }
        color[i] = mx;
    }
}

void combine_reduce(vector<int> &color, int l, int r, int mid, vector<vector<int>> &adj, int del)
{
    for (int i = mid + 1; i <= r; i++)
    {
        int mx = l;
        set<int> used;
        for (auto vert : adj[i])
        {
            used.insert(color[vert]);
        }
        while (used.find(mx) != used.end())
        {
            mx++;
        }
        color[i] = mx;
    }
    // for (int i = mid; i <= r; i++)
    // {
    //     int mx = l;
    //     set<int> used;
    //     for (auto vert : adj[i])
    //     {
    //         used.insert(color[vert]);
    //     }
    //     while (used.find(mx) != used.end())
    //     {
    //         mx++;
    //     }
    //     color[i] = mx;
    // }
}

void color_reduce_parallel(vector<int> &color, int l, int r, vector<vector<int>> &adj, int depth, int rank, int del)
{
    int mid = (l + r) / 2;
    int next_rank = rank + pow(2, depth);
    depth++;
    if (next_rank < nump)
    {
        send_vec(color, next_rank);
        int st = mid + 1;
        MPI_Send(&st, 1, MPI_INT, next_rank, send_data_tag, MPI_COMM_WORLD);
        MPI_Send(&r, 1, MPI_INT, next_rank, send_data_tag, MPI_COMM_WORLD);
        MPI_Send(&depth, 1, MPI_INT, next_rank, send_data_tag, MPI_COMM_WORLD);

        // cout << "Sent\n";
        // exit(0);

        color_reduce_parallel(color, l, mid, adj, depth, rank, del);

        vector<int> new_color;
        receive_vec(new_color, next_rank);
        for (int i = mid + 1; i <= r; i++)
        {
            color[i] = new_color[i];
        }
        combine_reduce(color, l, r, mid, adj, del);
    }
    else
    {
        color_reduce(color, l, r, adj, del);
    }
}

int main(int argc, char **argv)
{

    // freopen(argv[1], "r", stdin);
    // freopen(argv[2], "w", stdout);

    int rank, numprocs;

    /* start up MPI */
    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &numprocs);

    /*synchronize all processes*/
    MPI_Barrier(MPI_COMM_WORLD);
    double tbeg = MPI_Wtime();
    // vector<pair<int,int>> trans

    nump = numprocs;

    int del = 0;
    vector<int> arr;
    int n, m;
    if (rank == 0)
    {
        ifstream cin(argv[1]);
        cin >> n >> m;
        MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
        MPI_Bcast(&m, 1, MPI_INT, 0, MPI_COMM_WORLD);
        vector<pair<int, int>> edges;
        vector<int> aa, bb;
        for (int i = 0; i < m; i++)
        {
            int u, v;
            cin >> u >> v;
            aa.push_back(u);
            bb.push_back(v);
            edges.push_back({u, v});
        }

        // exit(0);

        for (int i = 1; i < nump; i++)
        {
            send_vec(aa, i);
            send_vec(bb, i);
        }

        vector<vector<int>> adj(m);
        for (int i = 0; i < m; i++)
        {
            for (int j = i + 1; j < m; j++)
            {
                if ((edges[i].first == edges[j].first || edges[i].second == edges[j].first || edges[i].first == edges[j].second || edges[i].second == edges[j].second))
                {
                    adj[i].push_back(j);
                    adj[j].push_back(i);
                }
            }
        }
        for (int i = 0; i < m; i++)
        {
            // for (auto neigh : adj[i])
            // {
            //     cout << neigh << " ";
            // }
            // cout << '\n';
            del = max(del, int(adj[i].size()));
        }
        cout << del << " ";
        MPI_Bcast(&del, 1, MPI_INT, 0, MPI_COMM_WORLD);
        vector<int> color(m, 0);
        for (int i = 0; i < m; i++)
            color[i] = i;
        // color_reduce(color, 0, m - 1, adj, del);
        // cout << "del = " << del << '\n';
        color_reduce_parallel(color, 0, m - 1, adj, 0, 0, del);
        set<int> all(color.begin(), color.end());
        ofstream cout(argv[2]);
        cout << all.size() << '\n';
        for (int i = 0; i < m; i++)
        {
            cout << color[i] + 1 << " ";
        }
        cout << '\n';
    }
    else
    {
        vector<int> aa, bb;
        receive_vec(aa, 0);
        receive_vec(bb, 0);
        vector<pair<int, int>> edges;
        for (int i = 0; i < m; i++)
        {
            int u, v;
            u = aa[i];
            v = bb[i];
            edges.push_back({u, v});
        }
        vector<vector<int>> adj(m);
        for (int i = 0; i < m; i++)
        {
            for (int j = i + 1; j < m; j++)
            {
                if ((edges[i].first == edges[j].first || edges[i].second == edges[j].first || edges[i].first == edges[j].second || edges[i].second == edges[j].second))
                {
                    adj[i].push_back(j);
                    adj[j].push_back(i);
                }
            }
        }
        // exit(0);
        vector<int> color;
        int rr = 1;
        while (rr <= rank)
        {
            rr *= 2;
        }
        rr /= 2;
        int sender = rank - rr;

        receive_vec(color, sender);
        int l, r, depth;
        MPI_Status stat;
        MPI_Recv(&l, 1, MPI_INT, sender, send_data_tag, MPI_COMM_WORLD, &stat);
        MPI_Recv(&r, 1, MPI_INT, sender, send_data_tag, MPI_COMM_WORLD, &stat);
        MPI_Recv(&depth, 1, MPI_INT, sender, send_data_tag, MPI_COMM_WORLD, &stat);

        color_reduce_parallel(color, l, r, adj, depth, rank, del);

        send_vec(color, sender);
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