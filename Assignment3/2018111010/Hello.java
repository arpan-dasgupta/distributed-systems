import java.rmi.Remote; 
import java.rmi.RemoteException;  

public interface Hello extends Remote { 
   String addGraph(String identifier, int numNodes) throws RemoteException;
   String addEdge(String identifier, int v1, int v2, int w) throws RemoteException;
   int getMST(String identifier) throws RemoteException;
   int checkConcurrency(String identifier) throws RemoteException;
} 
