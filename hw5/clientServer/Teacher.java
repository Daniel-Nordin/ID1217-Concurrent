package hw5.clientServer;

import java.rmi.*;
import java.rmi.registry.*;

public class Teacher {
    
    public static void main(String[] args) {
        try
        {
            RemoteInterface remote = new LookForPartner(5);

            LocateRegistry.createRegistry(1900);

            Naming.rebind("rmi://localhost:1900" + "/teacher", remote);
        }
        catch(Exception e){
            System.out.println(e);
        }
    }
}
