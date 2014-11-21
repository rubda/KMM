import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.concurrent.Semaphore;

public class SensorCommunication extends Communication {

    Semaphore sensorSemaphore = new Semaphore(1);
    static OutputStream outStream;

    int[] sensors = new int[7];

    public int getSensorValue(int sensor) {
        int value;
        try {
            sensorSemaphore.acquire();
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
        value = sensors[sensor];
        sensorSemaphore.release();
        return value;
    }

    public SensorCommunication(InputStream inStream, OutputStream outStream) {
        super(inStream);
        this.outStream = outStream;
    }

    @Override
    void receive(String inputString) {
        System.out.println("ReceiveSensors: " + inputString);
        inputString = inputString.substring(0, inputString.length()-1);
        String parts[] = inputString.split(":");

        if(parts[0].equals("#rotate")){
            //nånting



            //skicka sensordata till dator
            ComputerCommunication.send(inputString+";");
            send("#accept:true;");

        }
        else if(parts[0].equals("#distance")){

            try {
                sensorSemaphore.acquire();
            } catch (InterruptedException e) {
                e.printStackTrace();
            }


            if(parts.length>3){
                for (int i = 1; i < parts.length; i++) {
                 sensors[i] = (Integer.parseInt(parts[i]));
                }
            }
            else{
                sensors[Integer.parseInt(parts[1])] = Integer.parseInt(parts[2]);
            }

            sensorSemaphore.release();

            //skicka sensordata till dator
            ComputerCommunication.send(inputString+";");

            send("#accept:true;");
        }
        else if(parts[0].equals("#accept")){
            if(parts[1].equals("true")){
                //nånting

            }
            else if(parts[1].equals("false")){
                // nånting

            }
            else{
                System.out.println("FALSE: "+ inputString);
                send("#accept:false;");
            }
        }
        else{
            System.out.println("FALSE: "+ inputString);
            send("#accept:false;");
        }

    }

    public static void send(String message) {
        try {
            outStream.write(message.getBytes());
        }  catch (IOException ex) {
            System.err.println(ex.getMessage());
        }
    }
}
