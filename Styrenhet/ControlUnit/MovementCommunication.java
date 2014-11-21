import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;

public class MovementCommunication extends Communication {

    public static OutputStream outStream;

    public MovementCommunication(InputStream inStream, OutputStream outStream) {
        super(inStream);
        this.outStream = outStream;
    }

    @Override
    public void receive(String inputString) {
        System.out.println("ReceiveRobot: "+inputString);
        inputString = inputString.substring(0, inputString.length()-1);
        String parts[] = inputString.split(":");
        if(parts[0].equals("#init")){
            if(parts[1].equals("true")){
                //nånting
                Main.setRobotReady(true);
                send("#accept:true;");

            }
            else if(parts[1].equals("false")){
                Main.setRobotReady(false);
                send("#accept:true;");

            }
            else{
                System.out.println("FALSE: "+ inputString);
                send("#accept:false;");
            }

        }
        else if (parts[0].equals("#accept")){
            if(parts[1].equals("true")){
                //nånting


            }
            else if(parts[1].equals("false")){
                //nånting

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
