import java.util.*;
// Implementing the remote interface 

class Edge{
   int weight,vertex1,vertex2;
   Edge(int v1,int v2,int wt){
      vertex1 = v1;
      vertex2 = v2;
      weight = wt;
   }
}

class Graph{
   String identifier;
   int numNodes, numEdges;
   ArrayList<Edge> edges;
   // ArrayList<int> vertex1;
   // ArrayList<int> vertex2;

   Graph(int n)
   {
      numNodes = n;
   }

   public void addEdge(int v1, int v2, int wt)
   {
      Edge ee = new Edge(v1,v2,wt);
      edges.add(ee);
   }
}

public class ImplExample implements Hello {  
   Map<String, Graph> map = new HashMap<String, Graph>();  
   // Implementing the interface method 
   public String addGraph(String identifier, int numNodes)
   {
      Graph newg = new Graph(numNodes);
      map.put(identifier,newg);
      return "Ok";
   }
   public String addEdge(String identifier, int v1, int v2, int w){
      map.get(identifier).addEdge(v1,v2,w);
      System.out.println(map.get(identifier));

      return "Ok";
   }
   public int getMST(String identifier)
   {
      return -1;
   }
} 
