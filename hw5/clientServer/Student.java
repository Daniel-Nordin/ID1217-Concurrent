package hw5.clientServer;

import java.rmi.*;

public class Student {
    
    public static void main(String[] args) {
        String answer;
        int value = 7;
        try {
            RemoteInterface access = (RemoteInterface)Naming.lookup("rmi://localhost:1900" + "/teacher");
            answer = access.lookForPartner(value);
            System.out.println("ID: " + value + " answer: " + answer);
        } catch (Exception e) {
            System.out.println(e);
        }
    }
}
