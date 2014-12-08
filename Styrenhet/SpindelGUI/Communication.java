import gnu.io.SerialPortEvent;
import gnu.io.SerialPortEventListener;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.nio.charset.Charset;
import java.util.Arrays;

public class Communication implements SerialPortEventListener {

    static String[] parts;
    // Buffer to hold the reading
    private byte[] readBuffer = new byte[400];
    // I/O stream for serial port
    private InputStream inStream;
    private static OutputStream outStream;

    private static String lastAction;

    String message = "";

    public Communication(String message){
    }

    public Communication(InputStream inStream, OutputStream outStream) {
        this.inStream = inStream;
        this.outStream = outStream;
    }

    // Send parameters
    public static void sendParameter(String[] message){
        String send = "#param:"+message[0]+":"+message[1]+";";
        System.out.println(send);
        try {
            outStream.write(send.getBytes());
        } catch (IOException ex) {
            System.err.println(ex.getMessage());
        }
    }
    // Send action
    public static void sendAction(String message){
        System.out.println(message);
        if (!message.equals(lastAction)) {
            lastAction = message;
            try {
                outStream.write(message.getBytes());
            } catch (IOException ex) {
                System.err.println(ex.getMessage());
            }
        }
    }

    // Receive and interpret a message
    public static void receive(String message){
        //System.out.println("receive: "+message);
        message = message.substring(1, message.length()-1);
        parts = message.split(":");
        //System.out.println(parts[0]);
        if(parts[0].equals("distance")){

            if(parts.length>3){
                for (int i = 1; i < parts.length; i++) {
                    FrameWork.sensors[i].setData(parts[i]);
                    //FrameWork.textPanel.add("Sensor" + i + ": " + Integer.parseInt(parts[i]));
                }
            }
            else{
                FrameWork.sensors[Integer.parseInt(parts[1])].setData(parts[2]);
                //FrameWork.textPanel.add("Sensor" + Integer.parseInt(parts[1]) + ": " + Integer.parseInt(parts[2]));
            }
        }
        else if(parts[0].equals("rotate")){
            //System.out.println(parts[1]);
            FrameWork.gyro.setData(parts[1]);
            FrameWork.textPanel.add("Gyro: "+Integer.parseInt(parts[1]));
        }
        else if(parts[0].equals("mode")){
            System.out.println("mode setData to '" + parts[1] + "'");
            FrameWork.mode.setData(parts[1]);
            FrameWork.textPanel.add("Mode: "+parts[1]);
            if (parts[1].equals("manual")) {
                FrameWork.mainPanel.requestFocus();
            }
        }
        else if(parts[0].equals("time")){
            if(parts[1].equals("1")){
                FrameWork.watch.start();
            }
            else if(parts[1].equals("0")){
                FrameWork.watch.stop();
            }
        }
        else if(parts[0].equals("action")){
            FrameWork.textPanel.add("Styrbeslut: " + parts[1]);
        }
        else if(parts[0].equals("info")){
            FrameWork.textPanel.add("Info: " + parts[1]);
        }
        else{
            System.out.println(parts[0]);
            System.out.println("recieve() error");
        }

    }

    private void readSerial() {
        try {

            int availableBytes = inStream.available();
            if (availableBytes > 0) {
                // Read the serial port
                inStream.read(readBuffer, 0, availableBytes);
                // Print it out
                message = message + new String(readBuffer, 0, availableBytes);
                //System.out.println("Receive: " + message);

                message = receiveHelper(message);

            }
        } catch (IOException e) {
            System.out.println("readSerial() error");
        }
    }

    private String receiveHelper(String message){
        String messages[] = message.split(";", 2);
        if(messages.length == 2){
            receive(messages[0]+";");
            return receiveHelper(messages[1]);
        }
        return message;
    }


    public void serialEvent(SerialPortEvent events) {
        switch (events.getEventType()) {
            case SerialPortEvent.DATA_AVAILABLE:
                readSerial();
        }
    }

}


