import java.util.*;
// Implementing the remote interface 
public class ImplExample implements Hello {  
   
   // Implementing the interface method 
   public String printMsg() {  
      System.out.println("This is an example RMI program");  
      return "Hi";
   }  
} 
