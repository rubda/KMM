import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;

public class SensorCommunication extends Communication {

    static OutputStream outStream;

    public SensorCommunication(InputStream inStream, OutputStream outStream) {
        super(inStream);
        this.outStream = outStream;
    }

    @Override
    void receive(String inputString) {
        System.out.println("ReceiveSensors: "+inputString);
        send(inputString);
    }

    public static void send(String message) {
        try {
            outStream.write(message.getBytes());
        }  catch (IOException ex) {
            System.err.println(ex.getMessage());
        }
    }
}
