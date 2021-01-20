/* MPI Program Template */

#include <stdio.h>
#include <string.h>
#include "mpi.h"
#include <fstream>
#include <bits/stdc++.h>
using namespace std;
typedef long long int ll;

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

    int n;
    if (rank == 0)
    {
        cin >> n;
    }
    MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
    // cout << "Me " << rank << " " << n << '\n';

    double val = 0;
    for (int i = rank + 1; i <= n; i += numprocs)
    {
        val += 1 / (double(i * i));
    }
    double ans = 0;
    MPI_Reduce(&val, &ans, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
    if (rank == 0)
        cout << fixed << setprecision(6) << ans << " " << n << '\n';

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