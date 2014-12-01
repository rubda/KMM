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
            else if(parts[1].equals("movementstring")){
                System.out.println("SEND: " +parts[2]+":"+parts[3]);
                MovementCommunication.send(parts[2]+":"+parts[3]);
            }

            else if(parts[1].equals("speed")){     //INTE TESTAD
                MovementCommunication.send("#"+parts[1]+":"+parts[2]);
            }
            else if(parts[1].equals("Kp")){
                Main.Kp = Integer.parseInt(parts[2].substring(0,parts[2].length()-1));
            }
            else if(parts[1].equals("Kd")){
                Main.Kd = Integer.parseInt(parts[2].substring(0,parts[2].length()-1));
            }
            else if(parts[1].equals("Dt")){
                Main.Dt = Integer.parseInt(parts[2].substring(0,parts[2].length()-1));
            }
            else if(parts[1].equals("upperBound")){
                Main.upperBound = Integer.parseInt(parts[2].substring(0,parts[2].length()-1));
            }
            else if(parts[1].equals("lowerBound")){
                Main.lowerBound = Integer.parseInt(parts[2].substring(0,parts[2].length()-1));
            }
            else if(parts[1].equals("goalBound")){
                Main.goalBound = Integer.parseInt(parts[2].substring(0,parts[2].length()-1));
            }
            else if(parts[1].equals("stopBound")){
                Main.stopBound = Integer.parseInt(parts[2].substring(0,parts[2].length()-1));
            }
            else if(parts[1].equals("sensorDelay")){
                Main.sensorDelay = Integer.parseInt(parts[2].substring(0,parts[2].length()-1));
            }
            else if(parts[1].equals("walkToDistance")){
                System.out.println("send walkToDistance"+Integer.parseInt(parts[2].substring(0,parts[2].length()-1)));
                Main.walkToDistance(Integer.parseInt(parts[2].substring(0, parts[2].length() - 1)));
            }
            else if(parts[1].equals("rotateLeft")){
                Main.rotate(Integer.parseInt(parts[2].substring(0, parts[2].length() - 1)), "left");
            }
            else if(parts[1].equals("rotateRight")){
                Main.rotate(Integer.parseInt(parts[2].substring(0, parts[2].length() - 1)), "right");
            }
            else if(parts[1].equals("autoStart")){
                System.out.println("autoStart!");
                Main.auto();
            }
            else if(parts[1].equals("init")){                   //INTE TESTAD
                System.out.println("init!");
                MovementCommunication.send("#"+parts[1]+":0;");
            }

        }
        else {
            System.out.println("FEL från dator: "+inputString);
        }

    }

    public static void send(String message) {
        try {
            outStream.write(message.getBytes());
        }  catch (IOException ex) {
            System.out.println("Send error");
            System.err.println(ex.getMessage());
        }
    }
}
