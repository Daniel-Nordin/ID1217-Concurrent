package hw5.clientServer;

import java.rmi.Remote;
import java.rmi.RemoteException;

public interface RemoteInterface extends Remote {
    
    public String lookForPartner(int id) throws RemoteException;
}
