package hw4;

public class Female implements Runnable{
    public int id;
    private Bathroom bathroom;

    public Female(int id, Bathroom bathroom){
        this.id = id;
        this.bathroom = bathroom;
    }

    public void work(){
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
            bathroom.useBathrooom();
            bathroom.womanExit(this.id);
        }
    }
}