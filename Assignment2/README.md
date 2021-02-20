# Distributed Systems - Assignment 2 

## Question 1 : Ring Message passing in Erlang

- The input is taken using the `fread()` function from the file.
- The `n` processes are first created and the PID's are stored in the `Processes` variable.
- From this variable, the first PID (Process `0`) is removed and attached to the end, allowing the ring to work.
- Now, the initiator (root) send the message to the process `0` telling it to start the process. 
- Process `0` receives the message from root and starts the message passing process and waits for the message from process `n-1`.
- Each process `i` waits for process `i-1` to send the message and then removes the next person's PID from the list and forwards it to the process `i+1`.
- The procedure ends when process `0` receives an empty list from process `n-1`.

## Question 2 : Single Source Shortest Path in Erlang

- The input is taken using the `fread()` function from the file.
- The `Bellman Ford` Algorithm has been implemented for the problem. In short, the algorithm performs 'relaxation' process on every edge `n-1` times.
- The relaxation process updates the distance from source of either vertices on the edge using the weight of the edge.
- Here, this algorithm is parallelized by assigning the processes a certain number of edges and allowing only that process to relax those edges.
- At each iteration, we spawn `p` processes and tell each process to relax their edges and return the updated distances. A minimum of all these returned distances is taken.
- Finally the distance after all iterations is updated and printed.
- This algorithm should provide a speedup depending on number of processes used.