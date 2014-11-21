import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;

public class ComputerCommunication extends Communication {

    static OutputStream outStream;

    public ComputerCommunication(InputStream inStream, OutputStream outStream) {
        super(inStream);
        this.outStream = outStream;
    }

    public void receive(String inputString){
        System.out.println("ReceiveComputer: "+inputString);

        String parts[] = inputString.split(":");
        if(parts[0].equals("#walk") ||
                parts[0].equals("#rotate") ||
                parts[0].equals("#turn") ||
                parts[0].equals("#stop") ||
                parts[0].equals("#speed")){
            MovementCommunication.send(inputString);
        }
        else if(parts[0].equals("#param")){
            if(parts[1].equals("sensorstring")){

                System.out.println("SEND: " +parts[2]+":"+parts[3]);
                SensorCommunication.send(parts[2]+":"+parts[3]);
            }
        }
        else {   //SÅ länge!
            SensorCommunication.send(inputString);
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
