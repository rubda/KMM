import javax.swing.*;
import java.awt.event.ActionEvent;


public class FrameWork extends JFrame
{
    private final JFrame frame;  // The window itself
    private final JPanel sensorPanel, parameterPanel, panel3;
    private final MainComponent mainPanel;
    public static SensorComponent[] sensors; // Sensors
    public static SensorComponent gyro, mode; // Sensors
    private final ParameterComponent KP, KD, sensorstring;  // Parameters
    public static TextComponent textPanel;  // Decision log
    public static StopWatchRunner watch;  // Timer

    public FrameWork(){
        // Initialize window
        frame = new JFrame("SpindelGUI");
        frame.setDefaultCloseOperation(WindowConstants.EXIT_ON_CLOSE);
        frame.setResizable(false);

        // Initialize panels
        mainPanel = new MainComponent();
        mainPanel.setLayout(new BoxLayout(mainPanel, BoxLayout.LINE_AXIS));
        parameterPanel = new JPanel();
        parameterPanel.setLayout(new BoxLayout(parameterPanel, BoxLayout.PAGE_AXIS));
        sensorPanel = new JPanel();
        sensorPanel.setLayout(new BoxLayout(sensorPanel, BoxLayout.PAGE_AXIS));
        panel3 = new JPanel();
        panel3.setLayout(new BoxLayout(panel3, BoxLayout.PAGE_AXIS));

        // Initialize sensors
        sensors = new SensorComponent[7];
        for(int i = 1; i < 7; i++){
            sensors[i] = new SensorComponent("sensor"+i);
        }
        gyro = new SensorComponent("gyro");
        mode = new SensorComponent("mode");

        // Initialize decision log
        textPanel = new TextComponent();

        // Initialize parameters
        KP = new ParameterComponent("KP");
        KD = new ParameterComponent("KD");
        sensorstring = new ParameterComponent(("sensorstring"));

        // Add parameters
        parameterPanel.add(KP);
        parameterPanel.add(KD);
        parameterPanel.add(sensorstring);

        // Add sensors
        sensorPanel.add(mode);
        for(int i = 1; i < 7; i++){
            sensorPanel.add(sensors[i]);
        }
        sensorPanel.add(gyro);

        // Initialize and add the timer
        watch = new StopWatchRunner();
        panel3.add(watch);
        panel3.add(textPanel);


        //Pack to main panel
        mainPanel.add(parameterPanel);
        mainPanel.add(sensorPanel);
        mainPanel.add(panel3);

        // Pack frame
        frame.add(mainPanel);
        frame.pack();
        frame.setVisible(true);


        // Testing the receive functions
        Communication.receive("#time:1;");
        Communication.receive("#distance:2:23;");
        Communication.receive("#mode:auto;");
        Communication.receive("#action:SF;");
        Communication.receive("#distance:10:20:40:3:6:23;");
        Communication.receive("#rotate:84;");


    }
}