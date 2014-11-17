import java.io.InputStream;
import java.io.OutputStream;

/**
 * Created by danielfalk on 11/17/14.
 */
public class ComputerCommunication extends Communication {

    public ComputerCommunication(InputStream inStream, OutputStream outStream) {
        super(inStream, outStream);
    }

    public void receive(String inputString){
        String parts[] = inputString.split(":");
        if(parts[0].equals("#walk") ||
                parts[0].equals("#rotate") ||
                parts[0].equals("#turn") ||
                parts[0].equals("#stop")){
            RobotCommunication.send(inputString);
        }
    }


}
