import java.io.IOException;
import java.util.TooManyListenersException;

public class Main {

    public static void main(String[] args) throws IOException {
        FrameWork frameWork = new FrameWork();
        SerialPortHandler serialPortHandler = new SerialPortHandler();

        while(true){
            try {
                serialPortHandler.connect(args[0]);
                break;
            } catch (Exception e) {
                System.err.println(e.getMessage());
                continue;
            }
        }


        Communication communication =
                new Communication(serialPortHandler.getSerialInputStream(),
                        serialPortHandler.getSerialOutputStream());

        try {
            serialPortHandler.addDataAvailableListener(communication);
        } catch (TooManyListenersException ex) {
            System.err.println(ex.getMessage());
        }


    }
}

