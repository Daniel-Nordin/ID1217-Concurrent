package hw4;

import java.time.Instant;

public class Bathroom{
    int menInBathroom = 0;
    int womenInBathroom = 0;
    long startTime;
    long elapsedTime;

    public Bathroom(){
        this.menInBathroom = 0;
        this.womenInBathroom = 0;
        this.startTime = System.currentTimeMillis();
    }

    public synchronized void manEnter(int id){
        while(womenInBathroom > 0){
            try {
                wait();
            } catch (InterruptedException e) {
                // TODO Auto-generated catch block
                e.printStackTrace();
            }
        }
        menInBathroom++;
        elapsedTime = System.currentTimeMillis() - startTime;
        System.out.println(elapsedTime + " Male " + id + " is entering");
        notifyAll();
    }

    public synchronized void manExit(int id){
        menInBathroom--;
        elapsedTime = System.currentTimeMillis() - startTime;
        System.out.println(elapsedTime + " Male " + id + " is leaving");
        notifyAll();
    }

    public synchronized void womanEnter(int id){
        while(menInBathroom > 0){
            try {
                wait();
            } catch (InterruptedException e) {
                // TODO Auto-generated catch block
                e.printStackTrace();
            }
        }
        womenInBathroom++;
        elapsedTime = System.currentTimeMillis() - startTime;
        System.out.println(elapsedTime + " Female " + id + " is entering");
        notifyAll();
    }

    public synchronized void womanExit(int id){
        womenInBathroom--;
        elapsedTime = System.currentTimeMillis() - startTime;
        System.out.println(elapsedTime + " Female " + id + " is leaving");
        notifyAll();
    }

    public void useBathrooom(){
        try {
            Thread.sleep(500);
        } catch (InterruptedException e) {
            // TODO Auto-generated catch block
            e.printStackTrace();
        }
    }


    public static void main(String[] args) {
        Bathroom bathroom = new Bathroom();
        Thread m1 = new Thread(new Male(0, bathroom));
        m1.start();
        Thread m2 = new Thread(new Male(1, bathroom));
        m2.start();
        Thread f1 = new Thread(new Female(0, bathroom));
        f1.start();
        Thread f2 = new Thread(new Female(1, bathroom));
        f2.start();
    }

}