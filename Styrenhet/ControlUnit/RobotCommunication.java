import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;

public class RobotCommunication extends Communication {

    public static OutputStream outStream;

    public RobotCommunication(InputStream inStream, OutputStream outStream) {
        super(inStream);
        this.outStream = outStream;
    }

    @Override
    public void receive(String inputString) {
        System.out.println("ReceiveRobot: "+inputString);
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
