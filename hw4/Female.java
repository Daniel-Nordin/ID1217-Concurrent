package hw4;

import java.util.Random;

public class Female implements Runnable{
    public int id;
    private Bathroom bathroom;
    long workTime;
    long bathroomTime;

    public Female(int id, Bathroom bathroom){
        this.id = id;
        this.bathroom = bathroom;
    }

    public void work(){
        Random rng = new Random();
        this.workTime = rng.nextInt(3000);
        this.bathroomTime = rng.nextInt(1000);
        try {
            Thread.sleep(1000);
        } catch (InterruptedException e) {
            // TODO Auto-generated catch block
            e.printStackTrace();
        }
    }

    public void run(){
        while(true){
            work();
            bathroom.womanEnter(this.id);
            bathroom.useBathrooom(this.bathroomTime);
            bathroom.womanExit(this.id);
        }
    }
}