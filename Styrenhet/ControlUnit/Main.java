import java.util.TooManyListenersException;

public class Main {


    static ComputerCommunication ComputerCommunication;
    static SensorCommunication SensorCommunication;
    static MovementCommunication MovementCommunication;

    static Boolean robotReady = false;

    static Boolean rotating = false;
    static int upperBound = 120;
    static int lowerBound = 110;
    static int goalBound = 150;
    static int stopBound = 10;

    public static void main(String[] args) {
        if (args.length < 1) {
            System.out.println("Wrong arguments!");

            System.exit(1);
        }


        SerialHelper serialHelperComputer = new SerialHelper();
        SerialHelper serialHelperSensors = new SerialHelper();
        SerialHelper serialHelperMovement = new SerialHelper();

        checkListSerialPorts(serialHelperComputer);
        checkListSerialPorts(serialHelperSensors);
        checkListSerialPorts(serialHelperMovement);

        checkConnect(serialHelperComputer, args[0], 57600);
        checkConnect(serialHelperSensors, args[1], 9600);
        checkConnect(serialHelperMovement, args[2], 9600);


        //checkAddDataAvailableListener(serialHelper, args[1]);
        //System.out.println("Check data available listener");

        ComputerCommunication = new ComputerCommunication(serialHelperComputer.getSerialInputStream(), serialHelperComputer.getSerialOutputStream());
        try {
            serialHelperComputer.addDataAvailableListener(ComputerCommunication);

        } catch (TooManyListenersException ex) {
            System.err.println(ex.getMessage());
        }

        SensorCommunication = new SensorCommunication(serialHelperSensors.getSerialInputStream(), serialHelperSensors.getSerialOutputStream());
        try {
            serialHelperSensors.addDataAvailableListener(SensorCommunication);
        } catch (TooManyListenersException ex) {
            System.err.println(ex.getMessage());
        }

        MovementCommunication = new MovementCommunication(serialHelperMovement.getSerialInputStream(), serialHelperMovement.getSerialOutputStream());
        try {
            serialHelperMovement.addDataAvailableListener(MovementCommunication);
        } catch (TooManyListenersException ex) {
            System.err.println(ex.getMessage());
        }


        //checkDisconnect(serialHelper);
        // while(true){
        // }
    }

    private static String bla(String message){
        String messages[] = message.split(";", 2);
       // if(messages[0].charAt(messages[0].length() - 1) == (';')){
        if(messages.length == 2){
            System.out.println((messages[0]));

            return bla(messages[1]);
        }

        return message;
    }

    private static void checkListSerialPorts(SerialHelper serialHelper) {
        System.out.println("Check the listSerialPorts");
        String[] serialPorts = SerialHelper.listSerialPorts();
        if (serialPorts != null) {
            for (int i = 0; i < serialPorts.length; i++) {
                System.out.println("Port name: " + serialPorts[i]);
            }
        }
    }

    private static void checkConnect(SerialHelper serialHelper, String portName, int baudRate) {
        // Replace it with the tested serial port
        final String serialPort = portName;

        System.out.println("Connect to serial port: " + serialPort);
        try {
            serialHelper.connect(serialPort, baudRate);
        } catch (Exception e) {
            System.err.println(e.getMessage());
        }
    }

    private static void checkDisconnect(SerialHelper serialHelper) {
        System.out.println("Disconnect from serial port");
        serialHelper.disconnect();
        System.out.println("Disconnected");
    }

    public static void setRobotReady(Boolean robotReady) {
        Main.robotReady = robotReady;
    }


    public void auto(){
        boolean goal = false;

        MovementCommunication.send("#init:0;");
        //lås här och vänta på klartecken
        while(!robotReady){}

        while (!goal){
            updateSensors(0);


            if(SensorCommunication.getSensorValue(2) >= upperBound){
                     //kör fram
                MovementCommunication.send("#walk:f;");
            }
            else if(SensorCommunication.getSensorValue(2) <= lowerBound){

                //Gå fram till avstånd  ungefär 10 cm
                //såhär kan man göra men inte så snyggt...
                walkToDistance(stopBound);

                //Vad händer efter detta? Kolla upp!

            }
            else{
                //bättre väg åt sidan?

                //vänster > rakt fram
                if(SensorCommunication.getSensorValue(1)>SensorCommunication.getSensorValue(2)){
                    //Rotera vänster

                    rotate(90, "left");

                }
                //höger > rakt fram
                else if(SensorCommunication.getSensorValue(3)>SensorCommunication.getSensorValue(2)){
                    //Rotera höger

                    rotate(90, "right");
                }
                //Om inte bättre väg
                else{
                    //Kör fram
                    MovementCommunication.send("#walk:f;");
                }
            }

            //Om avstånd > 150. framåt, vänster, höger
            if(SensorCommunication.getSensorValue(1) < goalBound &&
                    SensorCommunication.getSensorValue(2) < goalBound &&
                    SensorCommunication.getSensorValue(3) < goalBound){
                //Mål!
                goal = true;
                MovementCommunication.send("#stop:after;");

            }

        }
    }

    public static void walkToDistance(int stopBound){
        ComputerCommunication.send("info:walkToDistance "+stopBound+";");
        MovementCommunication.send("#walk:f;");
        while(SensorCommunication.getSensorValue(1)>stopBound){    //10 är nog för lite!
            updateSensors(1);
        }
        MovementCommunication.send("#stop.after;");
        ComputerCommunication.send("info:stop;");

    }

    public static void rotate(int degrees, String direction){
        if(direction.equals("left")){
            ComputerCommunication.send("info:rotate left "+degrees+";");

            SensorCommunication.send("#rotate:"+degrees+";");
            MovementCommunication.send("#rotate:l;");
            rotating = true; //vänta på accept?
            while(rotating){}
            MovementCommunication.send("#stop:after;"); //ska vara stop direkt sen
        }
        else if(direction.equals("right")){
            ComputerCommunication.send("info:rotate right "+degrees+";");

            SensorCommunication.send("#rotate:-"+degrees+";");
            MovementCommunication.send("#rotate:r;");
            rotating = true; //vänta på accept?
            while(rotating){}
            MovementCommunication.send("#stop:after;"); //ska vara stop direkt sen
        }
        ComputerCommunication.send("info:stop;");
    }

    //update sensors. id 0 = all.
    private static void updateSensors(int id){             //INTE TESTAD
        SensorCommunication.send("#distance:" +id+";");
    }
}
