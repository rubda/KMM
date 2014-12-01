/**
 * Created by danielfalk on 11/26/14.
 */
public class regulatorThread extends Thread {

    private int Dt = 1000; //något standardvärde

    public void run() {
        while(true){




            try {
                Thread.sleep(Dt);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
    }


    public regulatorThread(){

        this.start();
    }

}
