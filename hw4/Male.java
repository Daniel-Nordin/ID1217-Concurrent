package hw4;

import java.util.Random;

public class Male implements Runnable{
    public int id;
    private Bathroom bathroom;
    private long workTime;
    private long bathroomTime;

    public Male(int id, Bathroom bathroom){
        this.id = id;
        this.bathroom = bathroom;
    }

    public void work(){
        Random rng = new Random();
        this.workTime = rng.nextInt(3000);
        this.bathroomTime = rng.nextInt(1000);
        try {
            Thread.sleep(this.workTime);
        } catch (InterruptedException e) {
            // TODO Auto-generated catch block
            e.printStackTrace();
        }
    }

    public void run(){
        while(true){
            work();
            bathroom.manEnter(this.id);
            bathroom.useBathrooom(this.bathroomTime);
            bathroom.manExit(this.id);
        }
    }
}