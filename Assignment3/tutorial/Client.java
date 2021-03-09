import java.rmi.registry.LocateRegistry; 
import java.rmi.registry.Registry;  
import java.util.*;

public class Client {  
   private Client() {}  
   public static void main(String[] args) {  
      while(true){
         Scanner obj = new Scanner(System.in);
         // System.out.println("Input command");
         String command = obj.next();

         try {  
            // Getting the registry 
            Registry registry = LocateRegistry.getRegistry(args[0],Integer.parseInt(args[1])); 
      
            // Looking up the registry for the remote object 
            Hello stub = (Hello) registry.lookup("Hello"); 
      
            // Calling the remote method using the obtained object according to the correct 
            if(command.equals("add_graph"))
            {
               String identifier = obj.next();
               int numNodes = obj.nextInt();
               String str = stub.addGraph(identifier,numNodes);
               // System.err.println(str);
            }
            else if(command.equals("add_edge"))
            {
               String identifier = obj.next();
               int v1 = obj.nextInt();
               int v2 = obj.nextInt();
               int w = obj.nextInt();
               String str = stub.addEdge(identifier,v1,v2,w);
               // System.err.println(str);
            }
            else if(command.equals("get_mst"))
            {
               String identifier = obj.next();
               int val = stub.getMST(identifier);
               System.err.println(val);
            }

            // System.err.println(str);
            // System.out.println("Remote method invoked"); 
         } catch (Exception e) {
            System.err.println("Client exception: " + e.toString()); 
            e.printStackTrace(); 
         } 
      }
   } 
}
