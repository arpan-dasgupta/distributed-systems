# DS Assignment 1 Report

### General Points

- All the questions have been implemented using MPI with no specific number of processes in mind. The code will run on as many processes as required (and available), including 1.
- Only process zero does I/O in all questions

## Question 1

In this question, process zero first reads and broadcasts the value of `n` to all the processes using MPI_BCast.

Each process then gets their own rank number and total number of processes and loops to add up all the values of `i`th term where `i` is `(i-1) % num_proc == rank`. This means, when there are 11 processes, process 0 sums all values like 1,12,23,34,... and so on.

Each of them store their own value and then MPI_Reduce is used by process 0 to sum up all the values from the processors and print the final value.

## Question 2

## Question 3

For this problem, we find the required edge coloring for the original graph from the vertex coloring of the corresponding line graph. So, first we convert the graph into line graph by adding edges for vertices in the new graph if there is a common vertex between the edges in the original graph.

Now, we apply the `Kuhn-Wattenhofer color-reduction` algorithm to get the required coloring.