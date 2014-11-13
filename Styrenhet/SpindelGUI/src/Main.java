import java.io.File;
import java.io.IOException;
import java.io.OutputStream;
import java.lang.reflect.Array;


public class Main {

    String text;
    static String[] hej;
    public static void main(String[] args) throws IOException {
        FrameWork frameWork = new FrameWork();
        SerialPortHandler sph = new SerialPortHandler();
        hej = sph.listSerialPorts();
        for (int i = 0; i < hej.length; i++) {
            System.out.println(hej[i]);
        }
        System.out.println(hej[10]);
        sph.connect(hej[10]);

        String serialMessage = "AT\r\n";
        OutputStream outstream = sph.getSerialOutputStream();
        outstream.write(serialMessage.getBytes());

    }
}

