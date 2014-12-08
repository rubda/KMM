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
                parts[0].equals("#stop")){
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
                Main.speed = Integer.parseInt(parts[2].substring(0,parts[2].length()-1));
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
            /*else if(parts[1].equals("walkToDistance")){
                System.out.println("send walkToDistance"+Integer.parseInt(parts[2].substring(0,parts[2].length()-1)));
                Main.walkToDistance(Integer.parseInt(parts[2].substring(0, parts[2].length() - 1)));
            }  */
            else if(parts[1].equals("rotateLeft")){
                Main.rotateCorner(Integer.parseInt(parts[2].substring(0, parts[2].length() - 1)), "left", "left");
            }
            else if(parts[1].equals("rotateRight")){
                Main.rotateCorner(Integer.parseInt(parts[2].substring(0, parts[2].length() - 1)), "right", "right");
            }
            else if(parts[1].equals("changeMode")){
                System.out.println(Main.isAutoMode);
                if (Main.isAutoMode.compareAndSet(true,true)){
                    Main.isAutoMode.set(false);
                    send("#mode:manual;");
                }
                else {
                    Main.isAutoMode.set(true);
                    send("#mode:auto;");
                    //Main.auto();
                }
            }
            else if(parts[1].equals("init")){
                //System.out.println("Init!");
                MovementCommunication.send("#"+parts[1]+":0;");
            }
            /*else if(parts[1].equals("sensorThread")){
                //Main.sensorThread = new SensorThread();
            }
            else if(parts[1].equals("regulateThread")){
                //Main.regulatorThread = new regulatorThread();

            }*/
            else if(parts[1].equals("angles")){
                System.out.println("angles!");
                ComputerCommunication.send("#info:Left angle-" + Main.angle("left") + ";");
                ComputerCommunication.send("#info:Right angle-"+Main.angle("right")+";");
            }
            else if(parts[1].equals("angleLimit")){
                Main.angleLimit = Integer.parseInt(parts[2].substring(0,parts[2].length()-1));
            }
            else if(parts[1].equals("distanceLimit")){
                Main.distanceLimit = Integer.parseInt(parts[2].substring(0,parts[2].length()-1));
            }
            else if(parts[1].equals("allowedAngleError")){
                Main.allowedAngleError = Integer.parseInt(parts[2].substring(0,parts[2].length()-1));
            }
            else if(parts[1].equals("walkAfterRotationValue")){
                Main.walkAfterRotationValue = Integer.parseInt(parts[2].substring(0,parts[2].length()-1));
            }
        }
        else {
            System.out.println("Wrong message from computer: " + inputString);
        }

    }

    public static void send(String message) {
        System.out.println("Send to computer: "+message);
        try {
            outStream.write(message.getBytes());
        }  catch (IOException ex) {
            System.out.println("Send error");
            System.err.println(ex.getMessage());
        }
    }
}
