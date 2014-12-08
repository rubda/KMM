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
        System.out.println("ReceiveMovement: "+inputString);
        inputString = inputString.substring(0, inputString.length()-1);
        String parts[] = inputString.split(":");

        if (parts[0].equals("#mode")){
            if (parts[1].equals("change")){
                if (Main.isAutoMode.compareAndSet(true,true)) {
                    Main.isAutoMode.set(false);
                    ComputerCommunication.send("#mode:manual");
                }
                else {
                    Main.isAutoMode.set(true);
                    ComputerCommunication.send("#mode:auto");
                }
            }
            else {
                System.out.println("Wrong command: "+ inputString);
                send("#denied:fel;");
            }
        }
        else if (parts[0].equals("#accept")){
            if(parts[1].equals("walk")){
                //nånting
            }
            else if(parts[1].equals("rotate")){
                //nånting
            }
            else if(parts[1].equals("turn")){
                //nånting
            }
            else if(parts[1].equals("init")){
                Main.setIsRobotReady(true);
            }
            else if(parts[1].equals("speed")){
                //nånting
            }
            else if(parts[1].equals("stop")){
                //nånting
            }
            else{
                System.out.println("Wrong command from Movement: "+ inputString);
                send("#denied:lastcommand;");
            }
        }
        else if(parts[0].equals("#denied")){
             //skicka senaste kommando igen?
        }
        else{
            System.out.println("Wrong command from Movement: "+ inputString);
            send("#denied:lastcommand;");
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
