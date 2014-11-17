import java.io.InputStream;
import java.io.OutputStream;

/**
 * Created by danielfalk on 11/17/14.
 */
public class SensorCommunication extends Communication {
    public SensorCommunication(InputStream inStream, OutputStream outStream) {
        super(inStream, outStream);
    }

    @Override
    void receive(String inputString) {
        System.out.println("ReceiveSensors: "+inputString);
    }

}
