package hw5.clientServer;

import java.rmi.*;
import java.rmi.server.UnicastRemoteObject;

public class LookForPartner extends UnicastRemoteObject implements RemoteInterface{
    private int[] queue;
    private int index;
    private int next;

    LookForPartner(int numberOfStudents) throws RemoteException{
        super();
        this.queue = new int[numberOfStudents];
        this.next = 0;
        this.index = 0;
    }

    public String lookForPartner(int id) throws RemoteException{
        String respons;
        this.queue[index++] = id;
        if(next %2 == 1){
            respons = "your partner is " + this.queue[next - 1];
            next++;
        }
        else{
            next++;
            while(next %2 == 1){

            }
            respons = "your partner is " + this.queue[next - 1];
        }
        return respons;
    }
}