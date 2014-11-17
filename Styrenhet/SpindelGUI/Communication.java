import gnu.io.SerialPortEvent;
import gnu.io.SerialPortEventListener;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.TooManyListenersException;

public class Communication implements SerialPortEventListener {

    static String[] parts;
    /**
     * Buffer to hold the reading
     */
    private byte[] readBuffer = new byte[400];
    /**
     * I/O stream for serial port
     */
    private InputStream inStream;
    private static OutputStream outStream;

    private int lastindex = 0;

    public Communication(String message){
    }

    public Communication(InputStream inStream, OutputStream outStream) {
        this.inStream = inStream;
        this.outStream = outStream;


    }

    // Send parameters
    public static void sendParameter(String[] message){
        System.out.println("#param:"+message[0]+":"+message[1]+";");
    }
    // Send action
    public static void sendAction(String message){
        System.out.println(message);
        message = message+"\r"; //behövs detta?
       try {
           outStream.write(message.getBytes());
       }  catch (IOException ex) {
           System.err.println(ex.getMessage());
       }
    }

    // Receive and interpret a message
    public static void receive(String message){

        message = message.substring(1, message.length()-1);
        parts = message.split(":");
        if(parts[0].equals("distance")){
            FrameWork.sensors[Integer.parseInt(parts[1])].setData(parts[2]);
            FrameWork.textPanel.add("Sensor" + Integer.parseInt(parts[1]) + ": " + Integer.parseInt(parts[2]));
        }
        else if(parts[0].equals("mode")){
            FrameWork.mode.setData(parts[1]);
            FrameWork.textPanel.add("Mode: "+parts[1]);
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
        else{
            System.out.println("FEL!!!!");
        }

    }

    private void readSerial() {
        try {
            int availableBytes = inStream.available();
            if (availableBytes > 0) {
                // Read the serial port
                inStream.read(readBuffer, lastindex, availableBytes);

                // Print it out
                System.out.println("Recv :" +
                        new String(readBuffer, 0, availableBytes));

                if(readBuffer.toString().charAt(readBuffer.length) == (';')){

                    receive(new String(readBuffer, 0, availableBytes));
                    lastindex = 0;
                }
                else   {
                    lastindex = availableBytes;
                }

            }
        } catch (IOException e) {
        }
    }

    public void serialEvent(SerialPortEvent events) {
        switch (events.getEventType()) {
            case SerialPortEvent.DATA_AVAILABLE:
                readSerial();
        }
    }


}


