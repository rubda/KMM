import gnu.io.SerialPortEvent;
import gnu.io.SerialPortEventListener;

import java.awt.*;
import java.awt.event.InputEvent;
import java.io.*;
import java.util.EventObject;
import java.util.TooManyListenersException;


import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.util.List;
import java.util.Map.Entry;


/**
 * \brief Private inner class to test and debug the SerialHelper class
 */
public abstract class Communication implements SerialPortEventListener {

    /**
     * Buffer to hold the reading
     */
    private byte[] readBuffer = new byte[400];
    /**
     * I/O stream for serial port
     */
    public InputStream inStream;
    public static OutputStream outStream;

    public Communication(InputStream inStream, OutputStream outStream) {
        this.inStream = inStream;
        this.outStream = outStream;
    }

    private void readSerial() {
        try {
            int availableBytes = inStream.available();
            if (availableBytes > 0) {
                // Read the serial port
                inStream.read(readBuffer, 0, availableBytes);

                // Print it out
                System.out.println("Recv :" +
                        new String(readBuffer, 0, availableBytes));


                receive(new String(readBuffer, 0, availableBytes));

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

    public static void send(String message) {
        message = message+"\r"; //behövs detta?
        try {

            outStream.write(message.getBytes());
        }  catch (IOException ex) {
            System.err.println(ex.getMessage());
        }
    }

}