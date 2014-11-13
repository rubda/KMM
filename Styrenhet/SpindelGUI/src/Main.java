import java.io.File;
import java.io.IOException;
import java.io.OutputStream;
import java.lang.reflect.Array;
import java.util.TooManyListenersException;

import static java.lang.Thread.sleep;


public class Main {

    String text;
    static String[] hej;
    public static void main(String[] args) throws IOException {
        FrameWork frameWork = new FrameWork();
        SerialPortHandler serialPortHandler = new SerialPortHandler();
       // hej = serialPortHandler.listSerialPorts();
       // for (int i = 0; i < hej.length; i++) {
       //     System.out.println(hej[i]);
       // }
       // System.out.println(hej[10]);
        serialPortHandler.connect(args[0]);

        try {
            sleep(4000);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }

        Communication communication =
                new Communication(serialPortHandler.getSerialInputStream(),
                        serialPortHandler.getSerialOutputStream());

        try {
            serialPortHandler.addDataAvailableListener(communication);
        } catch (TooManyListenersException ex) {
            System.err.println(ex.getMessage());
        }

        while(true){

        }


        //String serialMessage = "AT\r\n";
        //OutputStream outstream = serialPortHandler.getSerialOutputStream();
        //outstream.write(serialMessage.getBytes());

    }
}

