import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;

/**
 * Created by danielfalk on 11/17/14.
 */
public class RobotCommunication extends Communication {

    public RobotCommunication(InputStream inStream, OutputStream outStream) {
        super(inStream, outStream);
    }

    @Override
    public void receive(String inputString) {
        System.out.println("ReceiveRobot: "+inputString);
    }


}
