# DS Assignment 1 Report

### General Points

- All the questions have been implemented using MPI with no specific number of processes in mind. The code will run on as many processes as required (and available), including 1.
- Only process zero does I/O in all questions

## Question 1

In this question, process zero first reads and broadcasts the value of `n` to all the processes using MPI_BCast.

Each process then gets their own rank number and total number of processes and loops to add up all the values of `i`th term where `i` is `(i-1) % num_proc == rank`. This means, when there are 11 processes, process 0 sums all values like 1,12,23,34,... and so on.

Each of them store their own value and then MPI_Reduce is used by process 0 to sum up all the values from the processors and print the final value.

## Question 2

For this problem, the quicksort algorihm has been parallelized. The algorithm followed is as follows - 
- starting at the top of the quicksort tree, divide the array into two halves using the `partition` algorithm.
- find the next process for that depth (for 0->1; 0->2, 1->3; 0->4, 1->5, 2->6, 3->7; ...) using the formula `next(rank,depth) = rank + 2^depth`.
- If the next process is within the number of processes available, give it one half of the partitioned array to sort and recurse on the other half yourself.
- If the next process calculated is invalid, we have reached the base of the tree, perform simple quicksort on the array section.

### Notes about Q2

- This algorithm can be theoretically be proven to give a improvement over the original quicksort algorithm as the work is divided between processes at each level. The theoretical bound is difficult to calculate as the number of processes and randomness of quicksort causes sufficient difficulty.
- According to the tests, the algorithm performs better than the original sometimes and worse sometimes due to the randomness in the partition function. If the partitioning is ideal, then the parallel algorithm performs better. Else, in a non ideal partitioning, it performs worse due to massage passing overheads.

## Question 3

For this problem, we find the required edge coloring for the original graph from the vertex coloring of the corresponding line graph. So, first we convert the graph into line graph by adding edges for vertices in the new graph if there is a common vertex between the edges in the original graph.

Now, we apply the `Kuhn-Wattenhofer color-reduction` algorithm to get the required coloring. This algorithm divides the nodes into smaller halves ie. subgraphs and allows each process to perform coloring on it individually. 

Let `del` be the max degree of a node in a line graph. After each process does the individual coloring on their subgraph (leaving at most `del + 1` colors), the subgraphs are combined two at a time. During this combination process, the number of colors is reduced from `2(del + 1)` to `del + 1`. This process is repeated till the entire graph is combined. This algorithm thus takes `O(del * log(n/del))` time instead of the usual `O(n+m)` for the non-parallel agorithm.

The full detail and analysis of the algorithm are difficult to cover in this report as it is extremely long and has several other sources already.

### Notes about Q3

- Several other small optimizations have been done (like just sending the input and the computing the adjacency list in each process independently to allow lesser sends) which have not been explicitly mentioned in this report.
- The speedup is not acheived as expected as the original algorithm assumes much more parallelization and the message passing add a lot of overhead.