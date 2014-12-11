import java.util.TooManyListenersException;
import java.util.concurrent.atomic.AtomicBoolean;

import static java.lang.Math.*;

public class Main {

    private static ComputerCommunication ComputerCommunication;
    private static SensorCommunication SensorCommunication;
    private static MovementCommunication MovementCommunication;

    static AtomicBoolean running = new AtomicBoolean(false);
    static AtomicBoolean isAutoMode = new AtomicBoolean(false);

    private static AtomicBoolean isRobotReady = new AtomicBoolean(false);
    private static AtomicBoolean areSensorsReady = new AtomicBoolean(false);
    private static AtomicBoolean isRotating = new AtomicBoolean(false);
    private static AtomicBoolean isRotateAccepted = new AtomicBoolean(false);

    private static int walkAfterRotationCounter = 0;
    private static int walkAfterRotationValue = 4000;

    private static int counter = 0;
    private static int counterLimit = 30;

    private static int upperBound = 120;
    private static int lowerBound = 90;
    private static int goalBound = 60;
    private static int stopBound = 45;
    private static int sensorDelay = 300;
    private static int speed = 200;
    private static int angleLimit = 12;
    private static int distanceToSideWallLimit = 24;
    private static int allowedAngleError = 25;
    private static int deadEndWallLimit = 50;

    private final static int ninetyDegrees = 85;
    private final static double sideSensorDistance = 10.0; //Distance between side sensors.

    public static void main(String[] args) {
        if (args.length < 3) {
            System.out.println("Wrong arguments!");
            System.exit(1);
        }

        // Connection
        SerialHelper serialHelperComputer = new SerialHelper();
        SerialHelper serialHelperSensors = new SerialHelper();
        SerialHelper serialHelperMovement = new SerialHelper();

        checkListSerialPorts(serialHelperComputer);
        checkListSerialPorts(serialHelperSensors);
        checkListSerialPorts(serialHelperMovement);

        checkConnect(serialHelperComputer, args[0], 57600);
        checkConnect(serialHelperSensors, args[1], 9600);
        checkConnect(serialHelperMovement, args[2], 9600);

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

        while(true){

            // Init för autonomt läge
            if(isAutoMode.compareAndSet(true,true)){

                sendToComputer("#info:AutoStart;");

                boolean goal = false;
                isRobotReady.set(false);
                areSensorsReady.set(false);
                walkAfterRotationCounter = 0;

                sendToMovement("#init:"+speed+";");

                updateSensors(0);
                while(isRobotReady.compareAndSet(false,false) && areSensorsReady.compareAndSet(false,false) && isAutoMode.compareAndSet(true,true));
                sendToComputer("#info:Robot ready;");
                sendToComputer("#time:1;");

                // Loop för autonomt läge
                while (!goal && isAutoMode.compareAndSet(true,true)){
                    try {
                        Thread.sleep(sensorDelay);
                    } catch (InterruptedException e) {
                        e.printStackTrace();
                    }

                    counter++;

                    if(walkAfterRotationCounter > 0){
                        walkAfterRotationCounter--;
                    }

                    sendToComputer("#info:Check distance;");
                    sendToComputer(("#info:WalkAfterRotationCounter="+ walkAfterRotationCounter +";"));
                    sendToComputer(("#info:Counter="+ counter +";"));


                    areSensorsReady.set(false);
                    updateSensors(0);
                    while(areSensorsReady.compareAndSet(false,false)&& isAutoMode.compareAndSet(true,true));  // wait for sensors to update

                    // Kontrollera målgång (om avstånd > goalBound. framåt, vänster, höger)
                    if(getSensorValue(1) > goalBound && getSensorValue(2) > goalBound && getSensorValue(3) > goalBound
                            && getSensorValue(4) > goalBound && getSensorValue(6) > goalBound && walkAfterRotationCounter == 0){
                        sendToMovement("#stop:after;");
                        sendToComputer("#time:0;");
                        sendToComputer("#info:Goal!;");
                        isAutoMode.set(false);
                        sendToComputer("#mode:manual;");
                        goal = true;
                    }

                    // Kontrollera om roboten ska köra framåt
                    else if(getSensorValue(2) >= upperBound){
                        walk('f');
                    }

                    // Kontrollera om roboten ska rotera (i hörn)
                    else if(getSensorValue(2) <= stopBound){
                        if(getSensorValue(1) < deadEndWallLimit && getSensorValue(3) < deadEndWallLimit){
                            //Rotera 180 grader
                            if(counter < counterLimit){
                                sendToComputer("#info:Dead End bc. wrong turn;");
                                rotateCornerAndCorrect(180, "left", "left");
                            }
                            else{
                                sendToComputer("#info:Dead End bc. missed turn;");
                                backOutOfDeadEnd();
                            }
                        }

                        else if(getSensorValue(1)>getSensorValue(3)){ //vänster > höger
                            //Rotera vänster
                            sendToComputer("#info:Sensor 1>3. "+getSensorValue(1)+">"+getSensorValue(3)+";");
                            rotateCornerAndCorrect(ninetyDegrees, "left", "left");
                        }
                        else{
                            //Rotera höger
                            sendToComputer("#info:Sensor 1<=3. "+getSensorValue(1)+"<="+getSensorValue(3)+";");
                            rotateCornerAndCorrect(ninetyDegrees, "right", "right");
                        }
                        walkAfterRotationCounter = walkAfterRotationValue/speed;
                        counter = 0;
                    }

                    // Kontrollera om roboten ska rotera (innan återvändsgränd)
                    else if (getSensorValue(2) >= lowerBound && walkAfterRotationCounter == 0){
                        sendToComputer("#info:Bättre väg åt sidan?;"); //bättre väg åt sidan?

                        if(getSensorValue(1)>getSensorValue(2)){ //vänster > rakt fram
                            sendToMovement("#stop:after;");
                            //Rotera vänster
                            sendToComputer("#info:Sensor 1>2. "+getSensorValue(1)+">"+getSensorValue(2)+";");
                            correctRotationBeforeJunction("left");
                            correctDistance();
                            rotate(ninetyDegrees, "left");
                            walkAfterRotationCounter = walkAfterRotationValue/speed;
                            counter = 0;
                        }

                        else if(getSensorValue(3)>getSensorValue(2)){ //höger > rakt fram
                            sendToMovement("#stop:after;");
                            //Rotera höger
                            sendToComputer("#info:Sensor 3>2. "+getSensorValue(3)+">"+getSensorValue(2)+";");
                            correctRotationBeforeJunction("right");
                            correctDistance();
                            rotate(ninetyDegrees, "right");
                            walkAfterRotationCounter = walkAfterRotationValue/speed;
                            counter = 0;
                        }

                        else{ //Om inte bättre väg åt sidan
                            walk('f');
                        }
                    }

                    // Gå framåt
                    else {
                        walk('f');
                    }

                }
                sendToComputer("#time:0;");
                sendToMovement("#stop:after;");
            }
            else{
                while(isAutoMode.compareAndSet(false,false));
            }
        }
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

    // Walk forward
    private static void walk(char direction){
        int angle;
        boolean inCorridor = getSensorValue(3) < 40 && getSensorValue(4) < 40 && getSensorValue(1) < 40 && getSensorValue(6) < 40;

        // Calculate angle against wall (must be in corridor with walls on both sides)
        if(abs(angle("left")) < abs(angle("right")) && inCorridor){
            angle = angle("left");
        }
        else if(inCorridor){
            angle = angle("right");
        }
        else {
            angle = 0;
        }

        // Check distance to wall
        if(getSensorValue(1)< distanceToSideWallLimit){ // Too close to the left wall?
            sendToComputer("#info:Wall strafe right;");
            sendToComputer("#info:Sensor 1 = " + getSensorValue(1) +";");
            if(direction == 'b'){
                sendToMovement("#walk:f:200;");
            }
            else{
                sendToMovement("#walk:f:340;");
            }
        }
        else if(getSensorValue(3)< distanceToSideWallLimit){ // Too close to the right wall?
            sendToComputer("#info:Wall strafe left;");
            sendToComputer("#info:Sensor 3 = " + getSensorValue(3) + ";");
            if(direction == 'b'){
                sendToMovement("#walk:f:160;");
            }
            else{
                sendToMovement("#walk:f:20;");
            }
        }

        // Check angle
        else if(abs(angle)>angleLimit){
            if(angle < 0){
                sendToComputer("#info:Angle rotate left;");
                sendToComputer("#info:Sensor 1 = " + getSensorValue(1) +";");
                sendToComputer("#info:Sensor 6 = " + getSensorValue(6) +";");
                rotate(angleLimit, "left");
            }
            else{
                sendToComputer("#info:Angle rotate right;");
                sendToComputer("#info:Sensor 3 = " + getSensorValue(3) +";");
                sendToComputer("#info:Sensor 4 = " + getSensorValue(4) +";");
                rotate(angleLimit, "right");
            }
        }
        else{
            sendToComputer("#info:Walk forward;");
            if(direction == 'b'){
                sendToMovement("#walk:f:180;");
            }
            else{
                sendToMovement("#walk:f:0;");
            }
        }
    }

    static void backOutOfDeadEnd(){
        try {
            Thread.sleep(sensorDelay);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
        areSensorsReady.set(false);
        updateSensors(0);
        while(areSensorsReady.compareAndSet(false,false));

        if (getSensorValue(2) >= lowerBound && isAutoMode.compareAndSet(true,true)) {
            sendToComputer("#info:Bättre väg åt sidan?;"); //bättre väg åt sidan?

            if (getSensorValue(1) > getSensorValue(3)) { // vänster > höger
                sendToMovement("#stop:after;");
                //Rotera vänster
                sendToComputer("#info:Sensor 1>3. " + getSensorValue(1) + ">" + getSensorValue(3) + ";");
                correctRotationBeforeJunction("left");
                correctDistance();
                rotate(ninetyDegrees, "left");
                walkAfterRotationCounter = walkAfterRotationValue/speed;
                counter = 0;
            }
            else{
                sendToMovement("#stop:after;");
                //Rotera höger
                sendToComputer("#info:Sensor 1<=3. " + getSensorValue(1) + "<=" + getSensorValue(3) + ";");
                correctRotationBeforeJunction("right");
                correctDistance();
                rotate(ninetyDegrees, "right");
                walkAfterRotationCounter = walkAfterRotationValue/speed;
                counter = 0;
            }
        }
        else if (isAutoMode.compareAndSet(true,true)){
            walk('b');
            backOutOfDeadEnd();
        }
    }


    static void rotate(int degrees, String direction){
        try {
            Thread.sleep(sensorDelay);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
        sendToComputer("#info:rotate " + direction + " " + degrees + ";");
        if(direction.equals("left")) {
            sendToSensor("#rotate:" + degrees + ";");
        }
        else if(direction.equals("right")) {
            sendToSensor("#rotate:-" + degrees + ";");
        }

        isRotateAccepted.set(false);  // Wait for accept
        while(isRotateAccepted.compareAndSet(false, false) && isAutoMode.compareAndSet(true,true));
        if (isRotateAccepted.compareAndSet(true, true)) {
            sendToMovement("#rotate:"+direction.substring(0,1)+";");
            isRotating.set(true);
            while (isRotating.compareAndSet(true, true) && isAutoMode.compareAndSet(true,true));
            sendToMovement("#stop:after;");

        }
        areSensorsReady.set(false);
        updateSensors(8);
        while(areSensorsReady.compareAndSet(false,false) && isAutoMode.compareAndSet(true,true));
        sendToComputer("#info:Stopped rotating;");
    }

    // Rotate corner
    public static void rotateCornerAndCorrect(int degrees, String direction, String curve){

        rotate(degrees, direction);

        int angle;


        if(curve.equals("left")){
            angle = angle("right");
            sendToComputer("#info:Sensor 3 "+getSensorValue(3)+";");
            sendToComputer("#info:Sensor 4 "+getSensorValue(4)+";");
        }
        else{
            angle = angle("left");
            sendToComputer("#info:Sensor 1 "+getSensorValue(1)+";");
            sendToComputer("#info:Sensor 6 "+getSensorValue(6)+";");
        }
        sendToComputer("#info:Angle "+angle+";");


        if(abs(angle) > allowedAngleError && isAutoMode.compareAndSet(true,true)){
            sendToComputer("#info:Correcting rotation;");
            if(angle > 0){
                rotateCornerAndCorrect(allowedAngleError, "right", curve);
            }
            else{
                rotateCornerAndCorrect(abs(allowedAngleError), "left", curve);
            }
        }
    }


    // Correct rotation before junction
    public static void correctRotationBeforeJunction(String curve){

        int angle;
        areSensorsReady.set(false);
        updateSensors(8);
        while(areSensorsReady.compareAndSet(false,false) && isAutoMode.compareAndSet(true,true));

        if(curve.equals("left")){
            angle = angle("right");
            sendToComputer("#info:Sensor 3 = "+getSensorValue(3)+";");
            sendToComputer("#info:Sensor 4 = "+getSensorValue(4)+";");
        }
        else{
            angle = angle("left");
            sendToComputer("#info:Sensor 1 = "+getSensorValue(1)+";");
            sendToComputer("#info:Sensor 6 = "+getSensorValue(6)+";");
        }
        sendToComputer("#info:Angle "+angle+";");


        if(abs(angle) > allowedAngleError && isAutoMode.compareAndSet(true,true)){
            sendToComputer("#info:Correcting rotation;");
            if(angle > 0){
                rotate(allowedAngleError, "right");
                correctRotationBeforeJunction(curve);
            }
            else{
                rotate(abs(allowedAngleError), "left");
                correctRotationBeforeJunction(curve);
            }

        }
    }

    //Correct distance
    private static void correctDistance(){

        areSensorsReady.set(false);
        updateSensors(8);
        while(areSensorsReady.compareAndSet(false,false) && isAutoMode.compareAndSet(true,true));

        if(getSensorValue(2) > 112 && isAutoMode.compareAndSet(true,true)){
            sendToComputer("#info:Correcting distance;");
            sendToComputer("#info:Sensor2 = "+getSensorValue(2)+";");
            sendToMovement("#walk:f:0;");

            correctDistance();
        }
        else if(getSensorValue(2) < 94 && isAutoMode.compareAndSet(true,true)){
            sendToComputer("#info:Correcting distance;");
            sendToComputer("#info:Sensor2 = " + getSensorValue(2) + ";");
            sendToMovement("#walk:f:180;");
            correctDistance();
        }
        else {
            sendToMovement("#stop:after;");
        }

    }

    // Calculate angle against walls on either side
    public static int angle(String direction){

        if(direction.equals("right")){
            return (int) toDegrees(atan((double)(getSensorValue(3) - getSensorValue(4)) / sideSensorDistance));
        }
        else if(direction.equals("left")){
            return (int) toDegrees(atan((double) (getSensorValue(6) - getSensorValue(1)) / sideSensorDistance));
        }
        else{
            System.out.println("Wrong argument");
            return 0;
        }
    }


    // Setters

    public static void setIsRobotReady(Boolean value) {
        isRobotReady.set(value);
    }
    public static void setAreSensorsReady(Boolean value){
        areSensorsReady.set(value);
    }
    public static void setIsRotating(Boolean value){
        isRotating.set(value);
    }
    public static void setIsRotateAccepted(Boolean value){
        isRotateAccepted.set(value);
    }
    public static void setSpeed(int value){
        speed = value;
    }
    public static void setUpperBound(int value){
        upperBound = value;
    }
    public static void setLowerBound(int value){
        lowerBound = value;
    }
    public static void setGoalBound(int value){
        goalBound = value;
    }
    public static void setStopBound(int value){
        stopBound = value;
    }
    public static void setSensorDelay(int value){
        sensorDelay = value;
    }
    public static void setAllowedAngleError(int value){
        allowedAngleError = value;
    }
    public static void setAngleLimit(int value){
        angleLimit = value;
    }
    public static void setDistanceToSideWallLimit(int value){
        distanceToSideWallLimit = value;
    }
    public static void setWalkAfterRotationValue(int value){
        walkAfterRotationValue = value;
    }
    public static void setCounterLimit(int counterLimit) {
        counterLimit = counterLimit;
    }
    public static void setDeadEndWallLimit(int deadEndWallLimit) {
        deadEndWallLimit = deadEndWallLimit;
    }



    // "Shortcut" functions

    // Update sensors. id 0 = all.
    static void updateSensors(int id){
        sendToSensor("#distance:" +id+";");
    }

    // Get sensor values. 1 <= i <= 6
    static int getSensorValue(int i) {
        return SensorCommunication.getSensorValue(i);
    }

    // Shortcut to all send functions
    static void sendToComputer(String message) {
        ComputerCommunication.send(message);
    }
    static void sendToSensor(String message) {
        SensorCommunication.send(message);
    }
    static void sendToMovement(String message){
        MovementCommunication.send(message);
    }
}
