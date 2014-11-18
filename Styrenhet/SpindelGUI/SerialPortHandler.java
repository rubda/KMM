import gnu.io.*;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.ArrayList;
import java.util.Enumeration;
import java.util.TooManyListenersException;

public class SerialPortHandler {
    private SerialPort serialPort;
    private OutputStream outStream;
    private InputStream inStream;

    public void connect(String portName) throws IOException {
        try {
            // Obtain a CommPortIdentifier object for the port you want to open
            CommPortIdentifier portId =
                    CommPortIdentifier.getPortIdentifier(portName);

            // Get the port's ownership
            serialPort =
                    (SerialPort) portId.open("Demo application", 5000);

            // Set the parameters of the connection.
            setSerialPortParameters();

            // Open the input and output streams for the connection. If they won't
            // open, close the port before throwing an exception.
            outStream = serialPort.getOutputStream();
            inStream = serialPort.getInputStream();

        } catch (NoSuchPortException e) {
            throw new IOException(e.getMessage());
        } catch (PortInUseException e) {
            throw new IOException(e.getMessage());
        } catch (IOException e) {
            serialPort.close();
            throw e;
        }

    }

    // Get the serial port input stream
    // @return The serial port input stream
    public InputStream getSerialInputStream() {
        return inStream;
    }

    // Get the serial port output stream
    // @return The serial port output stream
    public OutputStream getSerialOutputStream() {
        return outStream;
    }

    // Sets the serial port parameters
    private void setSerialPortParameters() throws IOException {
        int baudRate = 57600; // 57600bps

        try {
            // Set serial port to 57600bps-8N1..my favourite
            serialPort.setSerialPortParams(
                    baudRate,
                    SerialPort.DATABITS_8,
                    SerialPort.STOPBITS_1,
                    SerialPort.PARITY_NONE);

            serialPort.setFlowControlMode(
                    SerialPort.FLOWCONTROL_NONE);
        } catch (UnsupportedCommOperationException ex) {
            throw new IOException("Unsupported serial port parameter");
        }
    }
    public static String[] listSerialPorts() {

        Enumeration ports = CommPortIdentifier.getPortIdentifiers();
        ArrayList portList = new ArrayList();
        String portArray[] = null;
        while (ports.hasMoreElements()) {
            CommPortIdentifier port = (CommPortIdentifier) ports.nextElement();
            //System.out.println(port.getName());
            if (port.getPortType() == CommPortIdentifier.PORT_SERIAL) {
                portList.add(port.getName());
            }
        }
        portArray = (String[]) portList.toArray(new String[0]);
        return portArray;
    }

    // brief Register listener for data available event
    // @param dataAvailableListener The data available listener
    public void addDataAvailableListener(SerialPortEventListener dataAvailableListener)
            throws TooManyListenersException {
        // Add the serial port event listener
        serialPort.addEventListener(dataAvailableListener);
        serialPort.notifyOnDataAvailable(true);
    }

    // brief Disconnect the serial port
    public void disconnect() {
        if (serialPort != null) {
            try {
                // close the i/o streams.
                outStream.close();
                inStream.close();
            } catch (IOException ex) {
                // don't care
            }
            // Close the port.
            serialPort.close();
            serialPort = null;
        }
    }
}