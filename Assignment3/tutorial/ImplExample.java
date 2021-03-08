import java.util.*;
// Implementing the remote interface 

// implements Comparable<Edge>
class Edge {
   public int weight,vertex1,vertex2;
   Edge(int v1,int v2,int wt){
      vertex1 = v1;
      vertex2 = v2;
      weight = wt;
   }
   // @Override
   // public boolean compareTo(Edge e) {
   //    // usually toString should not be used,
   //    // instead one of the attributes or more in a comparator chain
   //    return weight<e.weight;
   // }
}

class Graph{
   String identifier;
   int numNodes, numEdges;
   ArrayList<Edge> edges;
   int[] parents;
   // ArrayList<int> vertex1;
   // ArrayList<int> vertex2;

   Graph(int n)
   {
      numNodes = n;
      edges = new ArrayList<Edge>();
   }

   int 

   public void addEdge(int v1, int v2, int wt)
   {
      Edge ee = new Edge(v1,v2,wt);
      edges.add(ee);
   }

   public void printEdges(){
      edges.forEach(a->{ //Here, we are using lambda expression  
         System.out.println(a.vertex1+" "+a.vertex2+" "+a.weight);  
      });  
      System.out.println("OKKK");
   }

   public int getMST()
   {
      if(edges.size()==0)
         return -1;
      edges.sort((a,b)-> (a.weight-b.weight));
      parents = new int[numNodes+1];
      for(int i=0;i<numNodes+1;i++)
         parents[i] = -1;
      
      edges.forEach(a->{ //Here, we are using lambda expression  
         int wt = a.weight;
         int v1 = a.vertex1;
         int v2 = a.vertex2;
         // check = 
      }); 

      return -1;
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
      Graph xx = map.get(identifier);
      // System.out.println(xx);
      // xx.printEdges();
      xx.addEdge(v1,v2,w);
      // xx.printEdges();
      // System.out.println(map.get(identifier));
      return "Ok";
   }
   public int getMST(String identifier)
   {
      Graph xx = map.get(identifier);
      xx.printEdges();
      xx.getMST();
      xx.printEdges();
      return -1;
   }
} 
