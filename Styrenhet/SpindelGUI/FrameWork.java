import javax.swing.*;
import javax.swing.plaf.basic.BasicOptionPaneUI;
import javax.swing.plaf.basic.DefaultMenuLayout;
import java.awt.*;


public class FrameWork extends JFrame
{
    private final JFrame frame;  // The window itself
    private final JPanel sensorPanel, parameterPanel, panel3, buttonPanel;
    private final MainComponent mainPanel;
    public static SensorComponent[] sensors; // Sensors
    public static SensorComponent gyro, mode; // Sensors
    private final ParameterComponent Kp, Kd, Dt, sensorstring, movementstring, speed, upperBound, lowerBound, goalBound, stopBound, sensorDelay, walkToDistance, rotateLeft, rotateRight;  // Parameters
    public static TextComponent textPanel;  // Decision log
    public static StopWatchRunner watch;  // Timer
    private ButtonComponent autoStart, init;


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
        buttonPanel = new JPanel();
        buttonPanel.setLayout(new BoxLayout(buttonPanel, BoxLayout.LINE_AXIS));
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
        Kp = new ParameterComponent("Kp");
        Kd = new ParameterComponent("Kd");
        Dt = new ParameterComponent("Dt");
        sensorstring = new ParameterComponent("sensorstring");
        movementstring = new ParameterComponent("movementstring");
        speed = new ParameterComponent("speed");
        upperBound = new ParameterComponent("upperBound");
        lowerBound = new ParameterComponent("lowerBound");
        goalBound = new ParameterComponent("goalBound");
        stopBound = new ParameterComponent("stopBound");
        sensorDelay = new ParameterComponent("sensorDelay");
        walkToDistance = new ParameterComponent("walkToDistance");
        rotateLeft = new ParameterComponent("rotateLeft");
        rotateRight = new ParameterComponent("rotateRight");

        // Initialize buttons
        autoStart = new ButtonComponent("autoStart");
        init = new ButtonComponent("init");

        // Add buttons
        buttonPanel.add(autoStart);
        buttonPanel.add(init);

        // Add parameters
        parameterPanel.add(Kp);
        parameterPanel.add(Kd);
        parameterPanel.add(Dt);
        parameterPanel.add(speed);
        parameterPanel.add(upperBound);
        parameterPanel.add(lowerBound);
        parameterPanel.add(goalBound);
        parameterPanel.add(stopBound);
        parameterPanel.add(sensorDelay);
        parameterPanel.add(sensorstring);
        parameterPanel.add(movementstring);
        parameterPanel.add(walkToDistance);
        parameterPanel.add(rotateLeft);
        parameterPanel.add(rotateRight);


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
        mainPanel.add(buttonPanel);

        // Pack frame
        frame.add(mainPanel);
        frame.pack();
        frame.setVisible(true);


        // Testing the receive functions
        //Communication.receive("#time:1;");
        //Communication.receive("#distance:2:23;");
        //Communication.receive("#mode:auto;");
        //Communication.receive("#action:SF;");
        //Communication.receive("#distance:10:20:40:3:6:23;");
        //Communication.receive("#rotate:84;");


    }
}