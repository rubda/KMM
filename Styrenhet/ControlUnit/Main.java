import java.util.TooManyListenersException;
import java.util.concurrent.atomic.AtomicBoolean;

import static java.lang.Math.*;

public class Main {

    // Communication devices
    private static ComputerCommunication ComputerCommunication;
    private static SensorCommunication SensorCommunication;
    private static MovementCommunication MovementCommunication;

    // Booleans
    static AtomicBoolean running = new AtomicBoolean(false);
    static AtomicBoolean isAutoMode = new AtomicBoolean(false);
    private static AtomicBoolean isRobotReady = new AtomicBoolean(false);
    private static AtomicBoolean areSensorsReady = new AtomicBoolean(false);
    private static AtomicBoolean isRotating = new AtomicBoolean(false);
    private static AtomicBoolean isRotateAccepted = new AtomicBoolean(false);

    private static int walkAfterRotationCounter = 0;  // So the robot walks
    private static int walkAfterRotationValue = 4000;

    private static int counter = 0;
    private static int counterLimit = 30;

    private static int distanceToWallUpperBound = 120;
    private static int distanceToWallLowerBound = 90;
    private static int goalBound = 60;  // Lowest distance for goal
    private static int stopBound = 45;
    private static int sensorDelay = 300;
    private static int speed = 200;
    private static int angleCorrectionLimit = 12;
    private static int distanceToSideWallLimit = 24;
    private static int allowedAngleError = 25;
    private static int deadEndWallLimit = 50;

    // Constants
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

        // Running loop
        while(true){

            // Init for auto mode
            if(isAutoMode.compareAndSet(true,true)){

                sendToComputer("#info:AutoStart;");

                // Init some values
                boolean goal = false;
                isRobotReady.set(false);
                areSensorsReady.set(false);
                walkAfterRotationCounter = 0;
                counter = 0;

                sendToMovement("#init:"+speed+";");  // Init legs and set speed

                updateSensors(0);  // Update sensors and wait until they're done
                while(isRobotReady.compareAndSet(false,false) && areSensorsReady.compareAndSet(false,false) && isAutoMode.compareAndSet(true,true));
                sendToComputer("#info:Robot ready;");
                sendToComputer("#time:1;");  // Start timer

                // Loop för auto mode
                while (!goal && isAutoMode.compareAndSet(true,true)){
                    // Delay so we don't spam the other modules (if we do they sometimes break)
                    try {
                        Thread.sleep(sensorDelay);
                    } catch (InterruptedException e) {
                        e.printStackTrace();
                    }

                    // Increment/Decrement counters
                    counter++;
                    if(walkAfterRotationCounter > 0){
                        walkAfterRotationCounter--;
                    }

                    sendToComputer("#info:Check distance;");
                    sendToComputer(("#info:WalkAfterRotationCounter="+ walkAfterRotationCounter +";"));
                    sendToComputer(("#info:Counter="+ counter +";"));

                    // Update sensors and wait until they're done
                    areSensorsReady.set(false);
                    updateSensors(0);
                    while(areSensorsReady.compareAndSet(false,false)&& isAutoMode.compareAndSet(true,true));  // wait for sensors to update

                    // Check for goal
                    if(getSensorValue(1) > goalBound && getSensorValue(2) > goalBound && getSensorValue(3) > goalBound
                            && getSensorValue(4) > goalBound && getSensorValue(6) > goalBound && walkAfterRotationCounter == 0){
                        sendToMovement("#stop:after;");
                        sendToComputer("#time:0;");
                        sendToComputer("#info:Goal!;");
                        isAutoMode.set(false);
                        sendToComputer("#mode:manual;");
                        goal = true;
                    }

                    // Check if the robot should rotate (in corner)
                    else if(getSensorValue(2) <= stopBound){
                        if(getSensorValue(1) < deadEndWallLimit && getSensorValue(3) < deadEndWallLimit){
                            // Rotate 180 degrees
                            if(counter < counterLimit){
                                sendToComputer("#info:Dead End bc. wrong turn;");
                                rotateCornerAndCorrect(180, "left", "left");
                            }
                            else{
                                sendToComputer("#info:Dead End bc. missed turn;");
                                backOutOfDeadEnd();
                            }
                        }

                        else if(getSensorValue(1)>getSensorValue(3)){ // Left > Right
                            // Rotate left
                            sendToComputer("#info:Sensor 1>3. "+getSensorValue(1)+">"+getSensorValue(3)+";");
                            rotateCornerAndCorrect(ninetyDegrees, "left", "left");
                        }
                        else{
                            // Rotate right
                            sendToComputer("#info:Sensor 1<=3. "+getSensorValue(1)+"<="+getSensorValue(3)+";");
                            rotateCornerAndCorrect(ninetyDegrees, "right", "right");
                        }
                        walkAfterRotationCounter = walkAfterRotationValue/speed;
                        counter = 0;
                    }

                    // Check if the robot should rotate (in junction)
                    else if (distanceToWallLowerBound <= getSensorValue(2) && getSensorValue(3) <= distanceToWallUpperBound && walkAfterRotationCounter == 0){
                        sendToComputer("#info:Bättre väg åt sidan?;");

                        if(getSensorValue(1)>getSensorValue(2)){ // Left > Forward
                            sendToMovement("#stop:after;");
                            // Rotate left
                            sendToComputer("#info:Sensor 1>2. "+getSensorValue(1)+">"+getSensorValue(2)+";");
                            correctRotationBeforeJunction("left");
                            correctDistance();
                            rotate(ninetyDegrees, "left");

                            // Reset counters
                            walkAfterRotationCounter = walkAfterRotationValue/speed;
                            counter = 0;
                        }

                        else if(getSensorValue(3)>getSensorValue(2)){ // Right > Forward
                            sendToMovement("#stop:after;");
                            // Rotate right
                            sendToComputer("#info:Sensor 3>2. "+getSensorValue(3)+">"+getSensorValue(2)+";");
                            correctRotationBeforeJunction("right");
                            correctDistance();
                            rotate(ninetyDegrees, "right");

                            // Reset counter
                            walkAfterRotationCounter = walkAfterRotationValue/speed;
                            counter = 0;
                        }

                        else{ // If not better to either side
                            walk('f');
                        }
                    }

                    // Walk forward
                    else {
                        walk('f');
                    }

                }
                // Change mode from auto to manual: stop timer and legs
                sendToComputer("#time:0;");
                sendToMovement("#stop:after;");
            }
            else{
                // While in manual mode, check for auto
                while(isAutoMode.compareAndSet(false,false));
            }
        }
    }

    // Check and list available serial ports
    private static void checkListSerialPorts(SerialHelper serialHelper) {
        System.out.println("Check the listSerialPorts");
        String[] serialPorts = SerialHelper.listSerialPorts();
        if (serialPorts != null) {
            for (int i = 0; i < serialPorts.length; i++) {
                System.out.println("Port name: " + serialPorts[i]);
            }
        }
    }

    // Connect to a port
    private static void checkConnect(SerialHelper serialHelper, String portName, int baudRate) {
        final String serialPort = portName;

        System.out.println("Connect to serial port: " + serialPort);
        try {
            serialHelper.connect(serialPort, baudRate);
        } catch (Exception e) {
            System.err.println(e.getMessage());
        }
    }

    // Disconnect
    private static void checkDisconnect(SerialHelper serialHelper) {
        System.out.println("Disconnect from serial port");
        serialHelper.disconnect();
        System.out.println("Disconnected");
    }


    // Walk forward
    private static void walk(char direction){
        int angle;  // Angle against side wall
        boolean inCorridor = getSensorValue(3) < 40 && getSensorValue(4) < 40 && getSensorValue(1) < 40 && getSensorValue(6) < 40;  // Is the robot considered in a corridor?

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
                // Strafe right backwards
                sendToMovement("#walk:f:200;");
            }
            else{
                // Strafe right forward
                sendToMovement("#walk:f:340;");
            }
        }
        else if(getSensorValue(3)< distanceToSideWallLimit){ // Too close to the right wall?
            sendToComputer("#info:Wall strafe left;");
            sendToComputer("#info:Sensor 3 = " + getSensorValue(3) + ";");
            if(direction == 'b'){
                // Strafe left backwards
                sendToMovement("#walk:f:160;");
            }
            else{
                // Strafe left forward
                sendToMovement("#walk:f:20;");
            }
        }

        // Check the robots angle against walls
        else if(abs(angle)> angleCorrectionLimit){
            if(angle < 0){
                // Correct left
                sendToComputer("#info:Angle rotate left;");
                sendToComputer("#info:Sensor 1 = " + getSensorValue(1) +";");
                sendToComputer("#info:Sensor 6 = " + getSensorValue(6) +";");
                rotate(angleCorrectionLimit, "left");
            }
            else{
                // Correct right
                sendToComputer("#info:Angle rotate right;");
                sendToComputer("#info:Sensor 3 = " + getSensorValue(3) +";");
                sendToComputer("#info:Sensor 4 = " + getSensorValue(4) +";");
                rotate(angleCorrectionLimit, "right");
            }
        }
        else{
            // If no correction is necessary, just walk straight
            sendToComputer("#info:Walk forward;");
            if(direction == 'b'){
                sendToMovement("#walk:f:180;");
            }
            else{
                sendToMovement("#walk:f:0;");
            }
        }
    }

    // Back out of dead end, recursive
    static void backOutOfDeadEnd(){
        // Delay so we don't spam the other modules (if we do they sometimes break)
        try {
            Thread.sleep(sensorDelay);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
        // Update sensors and wait until they're done
        areSensorsReady.set(false);
        updateSensors(0);
        while(areSensorsReady.compareAndSet(false,false));

        // If the robot backed far enough
        if (getSensorValue(2) >= distanceToWallLowerBound && isAutoMode.compareAndSet(true,true)) {
            sendToComputer("#info:Bättre väg åt sidan?;");

            if (getSensorValue(1) > getSensorValue(3)) { // Left > Right
                sendToMovement("#stop:after;");
                // Rotate left
                sendToComputer("#info:Sensor 1>3. " + getSensorValue(1) + ">" + getSensorValue(3) + ";");
                correctRotationBeforeJunction("left");
                correctDistance();
                rotate(ninetyDegrees, "left");

                // Reset counters
                walkAfterRotationCounter = walkAfterRotationValue/speed;
                counter = 0;
            }
            else{
                sendToMovement("#stop:after;");
                // Rotate right
                sendToComputer("#info:Sensor 1<=3. " + getSensorValue(1) + "<=" + getSensorValue(3) + ";");
                correctRotationBeforeJunction("right");
                correctDistance();
                rotate(ninetyDegrees, "right");

                // Reset counters
                walkAfterRotationCounter = walkAfterRotationValue/speed;
                counter = 0;
            }
        }
        // If the robot has not backed far enough, back up
        else if (isAutoMode.compareAndSet(true,true)){
            walk('b');
            backOutOfDeadEnd();
        }
    }

    // Basic rotate
    static void rotate(int degrees, String direction){
        // Delay so we don't spam the other modules (if we do they sometimes break)
        try {
            Thread.sleep(sensorDelay);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }

        sendToComputer("#info:rotate " + direction + " " + degrees + ";");
        if(direction.equals("left")) {
            // Tell the gyro that we want to rotate left
            sendToSensor("#rotate:" + degrees + ";");
        }
        else if(direction.equals("right")) {
            // Tell the gyro that we want to rotate right
            sendToSensor("#rotate:-" + degrees + ";");
        }

        isRotateAccepted.set(false);  // Wait for accept from sensor-unit
        while(isRotateAccepted.compareAndSet(false, false) && isAutoMode.compareAndSet(true,true));

        if (isRotateAccepted.compareAndSet(true, true)) {
            sendToMovement("#rotate:"+direction.substring(0,1)+";");  // Tell the legs to rotate
            // Wait for the sensors to say we've rotated far enough, then stop the legs
            isRotating.set(true);
            while (isRotating.compareAndSet(true, true) && isAutoMode.compareAndSet(true,true));
            sendToMovement("#stop:after;");

        }

        // After the robot has rotated, update sensors to get fresh values and wait until they're done
        areSensorsReady.set(false);
        updateSensors(8);
        while(areSensorsReady.compareAndSet(false,false) && isAutoMode.compareAndSet(true,true));
        sendToComputer("#info:Stopped rotating;");
    }

    // Rotate in corner, recursive correction
    public static void rotateCornerAndCorrect(int degrees, String direction, String curve){

        // Rotate
        rotate(degrees, direction);

        int angle; // Angle against wall

        if(curve.equals("left")){
            // If the robot rotated left, check angle on the right side
            angle = angle("right");
            sendToComputer("#info:Sensor 3 "+getSensorValue(3)+";");
            sendToComputer("#info:Sensor 4 "+getSensorValue(4)+";");
        }
        else{
            // If the robot rotated right, check angle on the left side
            angle = angle("left");
            sendToComputer("#info:Sensor 1 "+getSensorValue(1)+";");
            sendToComputer("#info:Sensor 6 "+getSensorValue(6)+";");
        }
        sendToComputer("#info:Angle "+angle+";");

        // Correct rotation recursively
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

    // Correct rotation before junction, recursively
    public static void correctRotationBeforeJunction(String direction){
        int angle;

        // Update sensors and wait until they're done
        areSensorsReady.set(false);
        updateSensors(8);
        while(areSensorsReady.compareAndSet(false,false) && isAutoMode.compareAndSet(true,true));

        if(direction.equals("left")){
            // If the robot shall rotate left, check angle on the right side
            angle = angle("right");
            sendToComputer("#info:Sensor 3 = "+getSensorValue(3)+";");
            sendToComputer("#info:Sensor 4 = "+getSensorValue(4)+";");
        }
        else{
            // If the robot shall rotate right, check angle on the left side
            angle = angle("left");
            sendToComputer("#info:Sensor 1 = "+getSensorValue(1)+";");
            sendToComputer("#info:Sensor 6 = "+getSensorValue(6)+";");
        }
        sendToComputer("#info:Angle "+angle+";");

        // If the robot needs to straighten up
        if(abs(angle) > allowedAngleError && isAutoMode.compareAndSet(true,true)){
            sendToComputer("#info:Correcting rotation;");
            if(angle > 0){
                rotate(allowedAngleError, "right");
                correctRotationBeforeJunction(direction);
            }
            else{
                rotate(abs(allowedAngleError), "left");
                correctRotationBeforeJunction(direction);
            }

        }
    }

    // Correct distance (in junction) recursively
    private static void correctDistance(){

        // Update sensors and wait until they're done
        areSensorsReady.set(false);
        updateSensors(8);
        while(areSensorsReady.compareAndSet(false,false) && isAutoMode.compareAndSet(true,true));

        // If too far away, walk forward and check again
        if(getSensorValue(2) > 112 && isAutoMode.compareAndSet(true,true)){
            sendToComputer("#info:Correcting distance;");
            sendToComputer("#info:Sensor2 = "+getSensorValue(2)+";");
            sendToMovement("#walk:f:0;");

            correctDistance();
        }
        // If too close, walk back and check again
        else if(getSensorValue(2) < 94 && isAutoMode.compareAndSet(true,true)){
            sendToComputer("#info:Correcting distance;");
            sendToComputer("#info:Sensor2 = " + getSensorValue(2) + ";");
            sendToMovement("#walk:f:180;");
            correctDistance();
        }
        // If within allowed distance, stop
        else {
            sendToMovement("#stop:after;");
        }

    }

    // Calculate angle against walls on either side
    public static int angle(String direction){

        if(direction.equals("right")){
            // Calculate on right side
            return (int) toDegrees(atan((double)(getSensorValue(3) - getSensorValue(4)) / sideSensorDistance));
        }
        else if(direction.equals("left")){
            // Calculate on left side
            return (int) toDegrees(atan((double) (getSensorValue(6) - getSensorValue(1)) / sideSensorDistance));
        }
        else{
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
    public static void setDistanceToWallUpperBound(int value){
        distanceToWallUpperBound = value;
    }
    public static void setDistanceToWallLowerBound(int value){
        distanceToWallLowerBound = value;
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
    public static void setAngleCorrectionLimit(int value){
        angleCorrectionLimit = value;
    }
    public static void setDistanceToSideWallLimit(int value){
        distanceToSideWallLimit = value;
    }
    public static void setWalkAfterRotationValue(int value){
        walkAfterRotationValue = value;
    }
    public static void setCounterLimit(int value) {
        counterLimit = value;
    }



    // "Shortcut" functions

    // Update sensors. id 0 = all. id 7 = all fresh, median of 5. id 8 = all fresh, median of three.
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
