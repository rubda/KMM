import gnu.io.SerialPortEvent;
import gnu.io.SerialPortEventListener;
import java.io.*;
import java.io.IOException;

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
            int availableBytes = inStream.available();
            if (availableBytes > 0) {
                // Read the serial port
                inStream.read(readBuffer, 0, availableBytes);
                // Print it out
                message = message + new String(readBuffer, 0, availableBytes);
                System.out.println("Receive: " + message);
                System.out.println("Last char: " + message.charAt(message.length() - 1));
                if(message.charAt(message.length() - 1) == (';')){
                    receive(message);
                    message = "";
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

    abstract void receive(String inputString);

}