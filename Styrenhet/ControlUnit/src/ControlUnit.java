import gnu.io.SerialPortEvent;
import gnu.io.SerialPortEventListener;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.TooManyListenersException;

/**
 * \brief Private inner class to test and debug the SerialHelper class
 */
public class ControlUnit implements SerialPortEventListener {

    public static void main(String[] args) {
        if (args.length < 1) {
            System.out.println("Usage: java ControlUnit <dataToBeSentToSerialPort>");
            System.exit(1);
        }

        SerialHelper serialHelper = new SerialHelper();

        checkListSerialPorts(serialHelper);
        checkConnect(serialHelper, args[0]);

        checkAddDataAvailableListener(serialHelper, args[1]);
        //checkDisconnect(serialHelper);
        while(true){

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

    private static void checkConnect(SerialHelper serialHelper, String portName) {
        // Replace it with the tested serial port
        final String serialPort = portName;

        System.out.println("Connect to serial port: " + serialPort);
        try {
            serialHelper.connect(serialPort);
        } catch (Exception e) {
            System.err.println(e.getMessage());
        }
    }

    private static void checkAddDataAvailableListener(
            SerialHelper serialHelper, String data) {
        System.out.println("Check data available listener");

        ControlUnit tester =
                new ControlUnit(serialHelper.getSerialInputStream(),
                        serialHelper.getSerialOutputStream());

        try {
            serialHelper.addDataAvailableListener(tester);
        } catch (TooManyListenersException ex) {
            System.err.println(ex.getMessage());
        }

        OutputStream outStream = serialHelper.getSerialOutputStream();
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
    }

    private static void checkDisconnect(SerialHelper serialHelper) {
        System.out.println("Disconnect from serial port");
        serialHelper.disconnect();
        System.out.println("Disconnected");
    }
    /**
     * Buffer to hold the reading
     */
    private byte[] readBuffer = new byte[400];
    /**
     * I/O stream for serial port
     */
    private InputStream inStream;
    private OutputStream outStream;

    public ControlUnit(InputStream inStream, OutputStream outStream) {
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

    private void receive(String inputString){
        String parts[] = inputString.split(":");
        if(parts[0].equals("#walk") ||
                parts[0].equals("#rotate") ||
                parts[0].equals("#turn") ||
                parts[0].equals("#stop")){
            sendToRobot(inputString);
        }
    }

    private void sendToRobot(String message){
        System.out.println("To Robot: "+message);
    }
}