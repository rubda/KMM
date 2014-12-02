public class regulatorThread extends Thread {

    public void run() {
        while(true){
            Main.steeringValue = Main.regulate(Main.getError(), Main.olderror);

            try {
                Thread.sleep(Main.Dt);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
    }

    public regulatorThread(){

        this.start();
    }

}
