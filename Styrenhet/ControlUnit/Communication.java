import gnu.io.SerialPortEvent;
import gnu.io.SerialPortEventListener;
import java.io.*;
import java.io.IOException;
import java.nio.charset.Charset;

// brief Private inner class to test and debug the SerialHelper class
public abstract class Communication implements SerialPortEventListener {

    // Buffer to hold the reading
    private byte[] readBuffer = new byte[400];

    // I/O stream for serial port
    public InputStream inStream;

    String message = "";

    public Communication(InputStream inStream) {
        this.inStream = inStream;
    }

    private void readSerial() {
        try {
            //System.out.println("readSerial");
            //System.out.println(new String(readBuffer, Charset.defaultCharset()));
            //System.out.println(inStream.available());

            int availableBytes = inStream.available();
            if (availableBytes > 0) {
                // Read the serial port
                inStream.read(readBuffer, 0, availableBytes);
                // Print it out
                message = message + new String(readBuffer, 0, availableBytes);
                System.out.println("Receive: " + message);
                System.out.println("Last char: " + message.charAt(message.length() - 1));


                message = receiveHelper(message);

            }
        } catch (IOException e) {
            System.out.println("error");
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
            default:
        }
    }

    abstract void receive(String inputString);

}