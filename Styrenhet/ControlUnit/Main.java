import java.util.TooManyListenersException;

/**
 * Created by danielfalk on 11/17/14.
 */
public class Main {

    public static void main(String[] args) {
        if (args.length < 1) {
            System.out.println("Wrong arguments!");
            System.exit(1);
        }


        SerialHelper serialHelperComputer = new SerialHelper();
        SerialHelper serialHelperSensors = new SerialHelper();
        SerialHelper serialHelperRobot = new SerialHelper();

        checkListSerialPorts(serialHelperComputer);
        checkListSerialPorts(serialHelperSensors);
        checkListSerialPorts(serialHelperRobot);

        checkConnect(serialHelperComputer, args[0], 57600);
        checkConnect(serialHelperSensors, args[1], 57600);
        checkConnect(serialHelperRobot, args[2], 9600);


        //checkAddDataAvailableListener(serialHelper, args[1]);
        //System.out.println("Check data available listener");

        ComputerCommunication ComputerCommunication =
                new ComputerCommunication(serialHelperComputer.getSerialInputStream(),
                        serialHelperComputer.getSerialOutputStream());

        try {
            serialHelperComputer.addDataAvailableListener(ComputerCommunication);

        } catch (TooManyListenersException ex) {
            System.err.println(ex.getMessage());
        }
        SensorCommunication SensorCommunication =
                new SensorCommunication(serialHelperSensors.getSerialInputStream(),
                        serialHelperSensors.getSerialOutputStream());

        try {
            serialHelperSensors.addDataAvailableListener(SensorCommunication);

        } catch (TooManyListenersException ex) {
            System.err.println(ex.getMessage());
        }
        RobotCommunication RobotCommunication =
                new RobotCommunication(serialHelperRobot.getSerialInputStream(),
                        serialHelperRobot.getSerialOutputStream());

        try {
            serialHelperRobot.addDataAvailableListener(RobotCommunication);

        } catch (TooManyListenersException ex) {
            System.err.println(ex.getMessage());
        }


        //checkDisconnect(serialHelper);
        // while(true){
        // }
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
//
//        OutputStream outStream = serialHelper.getSerialOutputStream();
//        data = data + "\r";
//        try {
//            outStream.write(data.getBytes());
//        } catch (IOException ex) {
//            System.err.println(ex.getMessage());
//        }
//
//        try {
//            // Sleep for 10-secs
//            Thread.sleep(10000);
//        } catch (InterruptedException ex) {
//        }
//    }

    private static void checkDisconnect(SerialHelper serialHelper) {
        System.out.println("Disconnect from serial port");
        serialHelper.disconnect();
        System.out.println("Disconnected");
    }



}
