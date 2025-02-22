package hw4;

public class Male implements Runnable{
    public int id;
    private Bathroom bathroom;

    public Male(int id, Bathroom bathroom){
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
            bathroom.manEnter(this.id);
            bathroom.useBathrooom();
            bathroom.manExit(this.id);
        }
    }
}