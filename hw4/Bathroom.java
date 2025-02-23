package hw4;


public class Bathroom{
    int menInBathroom = 0;
    int womenInBathroom = 0;
    long startTime;
    long elapsedTime;

    public Bathroom(){

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

    public void useBathrooom(long bathroomTime){
        try {
            Thread.sleep(bathroomTime);
        } catch (InterruptedException e) {
            // TODO Auto-generated catch block
            e.printStackTrace();
        }
    }


    public static void main(String[] args) {
        int men = 4;
        int women = 4;
        if(args.length > 0){
            men = Integer.parseInt(args[0]);
            women = Integer.parseInt(args[1]);
        }
        Bathroom bathroom = new Bathroom();
        for(int i = 0; i < men; i++){
            Thread male = new Thread(new Male(i, bathroom));
            male.start();
        }
        for(int i = 0; i < women; i++){
            Thread female = new Thread(new Female(i, bathroom));
            female.start();
        }
        
    }

}