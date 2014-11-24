import com.sun.corba.se.spi.activation._InitialNameServiceStub;

import java.util.TooManyListenersException;

public class Main {


    static ComputerCommunication ComputerCommunication;
    static SensorCommunication SensorCommunication;
    static MovementCommunication MovementCommunication;

    static Boolean robotReady = false;

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

/*    private static void checkAddDataAvailableListener(SerialHelper serialHelper, String data) {
        System.out.println("Check data available listener");

        ComputerCommunication tester =
                new ComputerCommunication(serialHelper.getSerialInputStream(),
                        serialHelper.getSerialOutputStream());

        try {
            serialHelper.addDataAvailableListener(tester);

        } catch (TooManyListenersException ex) {
            System.err.println(ex.getMessage());
        }*/

/*      OutputStream outStream = serialHelper.getSerialOutputStream();
        data = data + "\r";
        try {
            outStream.write(data.getBytes());
        } catch (IOException ex) {
            System.err.println(ex.getMessage());
        }

        try {
            // Sleep for 10-secs
            Thread.sleep(10000);
        } catch (InterruptedException ex) {
        }
    }*/

    private static void checkDisconnect(SerialHelper serialHelper) {
        System.out.println("Disconnect from serial port");
        serialHelper.disconnect();
        System.out.println("Disconnected");
    }

    public static void setRobotReady(Boolean robotReady) {
        Main.robotReady = robotReady;
    }
    void auto(){
        boolean goal = false;

        MovementCommunication.send("#init:0;");
        //lås här och vänta på klartecken
        while(!robotReady){}

        while (!goal){
            updateSensorArray();


            if(SensorCommunication.getSensorValue(2) >= 120){
                     //kör fram
                MovementCommunication.send("#walk:f;");
            }
            else if(SensorCommunication.getSensorValue(2) <= 110){
                //Gå fram till avstånd  ungefär 10 cm
            }
            else{
                //bättre väg åt sidan?

                //vänster > rakt fram
                if(SensorCommunication.getSensorValue(1)>SensorCommunication.getSensorValue(2)){
                    //Rotera vänster
                }
                //höger > rakt fram
                else if(SensorCommunication.getSensorValue(3)>SensorCommunication.getSensorValue(2)){
                    //Rotera höger
                }
                //Om inte bättre väg
                else{
                    //Kör fram
                    MovementCommunication.send("#walk:f;");
                }
            }

            //Om avstånd > 150. framåt, vänster, höger
            if(SensorCommunication.getSensorValue(1) < 150 &&
                    SensorCommunication.getSensorValue(2) < 150 &&
                    SensorCommunication.getSensorValue(3) < 150){
                //Mål!
                goal = true;
                MovementCommunication.send("#stop:after;");

            }

        }
    }

    private void updateSensorArray(){
        SensorCommunication.send("#distance:0;");
    }
}
