import java.util.TooManyListenersException;

import static java.lang.Math.*;


public class Main {


    static ComputerCommunication ComputerCommunication;
    static SensorCommunication SensorCommunication;
    static MovementCommunication MovementCommunication;

    static Boolean robotReady = false;
    static Boolean sensorsReady = false;

    static int width = 20;
    static int length = 24;

    static Boolean rotating = false;
    static Boolean acceptRotate = false;
    static int upperBound = 120;
    static int lowerBound = 110;
    static int goalBound = 100;
    static int stopBound = 40;
    static int sensorDelay = 500;
    static int sideSensorDistance = 10; //Distance between side sensors.
    static int rotateThreshold;
    static double lowerLimit = 1.0;
    static double upperLimit = 1.0;
    static double Kp = 1.0;
    static double Kd = 1.0;
    static int Dt = 500;
    static int sensorLimit;
    static int angleLimit = 10;
    int error;
    static int olderror = 0;
    static double steeringValue = 0;
    static long time;
    static Timer timer = new Timer();
    static SensorThread sensorThread;
    static regulatorThread regulatorThread;

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


    public static void auto(){
        ComputerCommunication.send("#info:autoStart;");

        boolean goal = false;
        robotReady = false;
        sensorsReady = false;

        MovementCommunication.send("#init:0;");
        sensorThread = new SensorThread();
        //regulatorThread = new regulatorThread();
        //sensorsReady = false;
        //updateSensors(0);
        //lås här och vänta på klartecken
        while(!robotReady && !sensorsReady){}
        ComputerCommunication.send("#info:Robot ready;");
        ComputerCommunication.send("#time:1;");

        while (!goal){

            try {
                Thread.sleep(sensorDelay);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }

            ComputerCommunication.send("#info:Check distance;");
            // wait for sensors to update
            sensorsReady = false;
            //updateSensors(0);
            while(!sensorsReady){}


            //Om avstånd > 100. framåt, vänster, höger
            if(SensorCommunication.getSensorValue(1) > goalBound &&
                    SensorCommunication.getSensorValue(2) > goalBound &&
                    SensorCommunication.getSensorValue(3) > goalBound){
                //Mål!?
                ComputerCommunication.send("#time:0;");
                ComputerCommunication.send("#info:Goal!;");
                goal = true;
                sensorThread.stop();
                //regulatorThread.stop();
                sensorThread.destroy();
                //regulatorThread.destroy();

                MovementCommunication.send("#stop:after;");
            }

            if(SensorCommunication.getSensorValue(2) >= upperBound){
                //kör fram
                walk();
            }
            else if(SensorCommunication.getSensorValue(2) <= stopBound){

                //Gå fram till avstånd  ungefär 10 cm
                //såhär kan man göra men inte så snyggt...
                //walkToDistance(stopBound);

                //avstånd
                //vänster > höger
                if(SensorCommunication.getSensorValue(1)>SensorCommunication.getSensorValue(3)){
                    //Rotera vänster
                    rotate(90, "left");
                }
                else{
                    //Rotera höger
                    rotate(90, "right");
                }
            }
            else if (SensorCommunication.getSensorValue(2) >= lowerBound){
                ComputerCommunication.send("#info:Bättre väg åt sidan?;");
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
                    walk();
                }
            }
            else {
                walk();
            }

        }
    }

    public static void setRobotReady(Boolean robotReady) {
        Main.robotReady = robotReady;
    }

    public static void setSensorsReady(Boolean sensorsReady){
        Main.sensorsReady = sensorsReady;
    }

    /*public static void walkToDistance(int stopBound){
        //updateSensors(2);
        System.out.println("walkToDistance "+stopBound);
        ComputerCommunication.send("#info:walkToDistance "+stopBound+";");
        //MovementCommunication.send("#walk:f;");
        while(SensorCommunication.getSensorValue(2)>stopBound){
            //updateSensors(2);
            walk();
            try {
                Thread.sleep(sensorDelay);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
        MovementCommunication.send("#stop:after;");
        ComputerCommunication.send("#info:stop;");
    } */

    private static void walk(){
/*        if(steeringValue < lowerLimit){
            MovementCommunication.send("#turn:l;");
            ComputerCommunication.send("#info:Turn left;");
        }
        else if(steeringValue > upperLimit){
            MovementCommunication.send("#turn:r;");
            ComputerCommunication.send("#info:Turn right;");
        }
        else{
            MovementCommunication.send("#walk:f;");
            ComputerCommunication.send("#info:Walk forward;");
        }*/

        int angle;
        int distance;

        if(SensorCommunication.getSensorValue(1)<SensorCommunication.getSensorValue(3)){
            distance = SensorCommunication.getSensorValue(1);
        }
        else{
            distance = SensorCommunication.getSensorValue(3);
        }


        if(abs(angle("left"))<abs(angle("right"))){
            angle = angle("left");
        }
        else{
            angle = angle("right");
        }

        if(abs(angle)>angleLimit){
            if(angle < 0){
                ComputerCommunication.send("#info:Turn left;");
                MovementCommunication.send("#turn:l;");
            }
            else{
                MovementCommunication.send("#turn:r;");
                ComputerCommunication.send("#info:Turn right;");
            }
        }
        else{
            MovementCommunication.send("#walk:f;");
            ComputerCommunication.send("#info:Walk forward;");
        }



    }

    public static void rotate(int degrees, String direction){
        if(direction.equals("left")){
            ComputerCommunication.send("#info:rotate left "+degrees+";");
            SensorCommunication.send("#rotate:"+degrees+";");

            //vänta på accept?
            timer.start();
            while(time <2000 && !acceptRotate);
            timer.stop();
            if (acceptRotate) {
                MovementCommunication.send("#rotate:l;");
                rotating = true;
                while (rotating) {}
                MovementCommunication.send("#stop:after;"); //ska vara stop direkt sen
                acceptRotate = false;
            }
            else{
                ComputerCommunication.send("#info:rotate again");
                rotate(degrees, direction);
            }
        }
        else if(direction.equals("right")){
            ComputerCommunication.send("#info:rotate right "+degrees+";");

            SensorCommunication.send("#rotate:-"+degrees+";");


            // vänta på accept
            timer.start();
            while(time <2000 && !acceptRotate);
            timer.stop();
            if (acceptRotate){
                MovementCommunication.send("#rotate:r;");
                rotating = true;
                while(rotating){}
                MovementCommunication.send("#stop:after;"); //ska vara stop direkt sen
                acceptRotate = false;
            }
            else{
                ComputerCommunication.send("#info:rotate again");
                rotate(degrees, direction);
            }
        }
        ComputerCommunication.send("#info:stop;");
    }

    //update sensors. id 0 = all.
    public static void updateSensors(int id){
        SensorCommunication.send("#distance:" +id+";");
    }

    public static int getError(){

        int left, right;

        left = SensorCommunication.getSensorValue(1);
        right = SensorCommunication.getSensorValue(3);

        if(left > sensorLimit && right < sensorLimit){
           return right-(80-right);
        }
        else if(right > sensorLimit && left < sensorLimit){
            return left-(80-left);
        }

        else{
            return right-left;
        }


    }

    public static double regulate(int error, int olderror){
        steeringValue = Kp*error+(Kd/Dt)*(double)(error-olderror);
        ComputerCommunication.send("#info:Regulation;");
        ComputerCommunication.send("#info:Error "+error+";");
        ComputerCommunication.send("#info:Kp*error "+Kp*error+";");
        ComputerCommunication.send("#info:(Kd/Dt)*(error-olderror) "+(Kd/Dt)*(double)(error-olderror)+";");
        ComputerCommunication.send("#info:SteeringValue "+steeringValue+";");

        Main.olderror = error;
        return steeringValue;
    }

    public static int angle(String direction){
        if(direction.equals("right")){
            System.out.println(SensorCommunication.getSensorValue(3) - SensorCommunication.getSensorValue(4));
            System.out.println((double)(SensorCommunication.getSensorValue(3) - SensorCommunication.getSensorValue(4)) / (double)sideSensorDistance);
            return (int) toDegrees(atan((double)(SensorCommunication.getSensorValue(3) - SensorCommunication.getSensorValue(4)) / (double)sideSensorDistance));
        }
        else if(direction.equals("left")){

            return (int) toDegrees(atan((double) (SensorCommunication.getSensorValue(6) - SensorCommunication.getSensorValue(1)) / (double) sideSensorDistance));
        }
        else{
            System.out.println("Wrong argument");
            return 0;
        }
    }
}
