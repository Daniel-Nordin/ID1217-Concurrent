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

    public synchronized String lookForPartner(int id) throws RemoteException{
        String respons;
        this.queue[index++] = id;
        if(next %2 == 1){
            respons = "your partner is " + this.queue[next - 1];
            next++;
            notifyAll();
        }
        else{
            if(index == queue.length){
                respons = "You will be alone";
            }else
            {
            next++;
            try {
                wait();
            } catch (InterruptedException e) {
                // TODO Auto-generated catch block
                e.printStackTrace();
            }
           
            respons = "your partner is " + this.queue[next - 1];
            notifyAll();
            }
        }
        return respons;
    }
}