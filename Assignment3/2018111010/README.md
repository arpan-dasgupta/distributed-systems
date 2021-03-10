# DS Assignment 3

The asssignment has been done in Java. The steps to run the code are :- 

`javac *.java`

`java Server 4000`

`java Client 0.0.0.0 4000`

## Architecture

- `Java RMI` has been used for the simple client server architecture which supports multiple clients. RMI stands for Remote Method Invocation. It is a mechanism that allows an object residing in one system (JVM) to access/invoke an object running on another JVM.
- RMI provides the ability for the client to remotely call functions which are provided by the server. Here the interface `Hello` is provided by the server and registered using the call `registry.bind("Hello", stub);` in the Server.java code.
- This interface contains the required functions for the Client to invoke and the functions are implemented in `ImplExample.java`.
- The client searches the registry for the interface `Hello` and then calls the functions after parsing the commands as required.

## Algorithm

- Java HashMap is used for mapping strings to the corect Graph objects.
- The algorithm used for MST calculation is `Kruskal's Algorithm`. It is an efficient algorithm which computes MST based on Disjoint Set Union (DSU) data structure. Its time complexity is `O(mlog(m) + mlog(n))` where `n = number of nodes`, `m = number of edges`.
- The algorithm in short is - sort edges based on increasing order of weights, pick an edge to be a part of the MST if it connects two disjoint components.
- The latter part is done efficiently using the DSU.

## Results

- The algorithm and the structure work well, but without some sort of locking, the server might run into race conditions. To avoid this, `synchronize` in Java has bee used which prevents two threads from changing a certain variable together.
- The HashMap is locked using synchrnize (as multiple accesses might cause issues) and this ensures that no two processes ever clash during execution.
- Now, the entire structure works perfectly.

## Observations

- The issue of concurrency was studied in detail by using an empty loop and studying the entry and exit times of clients using the `checkConcurrency` function.
- Without this, race conditions would persist due to simultaneous access by the Clients on the HashMap.

## Assumptions

The MST of a graph with no proper MST is -1. For example, input
```
add_graph gg 12
add_edge gg 1 2 3
get_mst gg
```
gives -1.