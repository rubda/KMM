public class SensorThread extends Thread {

    public void run() {
        while(true){
            Main.updateSensors(0);
            try {
                Thread.sleep(Main.sensorDelay);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
    }

    public SensorThread(){
        this.start();
    }

}