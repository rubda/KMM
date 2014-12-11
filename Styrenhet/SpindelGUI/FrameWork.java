import javax.swing.*;
import java.awt.*;


public class FrameWork extends JFrame
{
    private final JFrame frame;  // The window itself
    private final JPanel sensorPanel, parameterPanel, panel3, buttonPanel;
    public static final MainComponent mainPanel = new MainComponent();
    public static SensorComponent[] sensors = new SensorComponent[7]; // Sensors
    public static SensorComponent gyro, mode;
    private final ParameterComponent counterLimit, walkAfterRotationValue, allowedAngleError, angleLimit, distanceToSideWallLimit, sensorstring, movementstring, speed, distanceToWallUpperBound, distanceToWallLowerBound, goalBound, stopBound, sensorDelay;  // Parameters
    public static TextComponent textPanel;  // Decision log
    public static StopWatchRunner watch;  // Timer
    private ButtonComponent changeMode, init, angels; // Buttons


    public FrameWork(){
        // Initialize window
        frame = new JFrame("SpindelGUI");
        frame.setDefaultCloseOperation(WindowConstants.EXIT_ON_CLOSE);
        frame.setResizable(false);

        // Initialize panels
        mainPanel.setLayout(new BoxLayout(mainPanel, BoxLayout.LINE_AXIS));
        parameterPanel = new JPanel();
        parameterPanel.setLayout(new BoxLayout(parameterPanel, BoxLayout.PAGE_AXIS));
        parameterPanel.setPreferredSize(new Dimension(300, 500));
        sensorPanel = new JPanel();
        sensorPanel.setLayout(new BoxLayout(sensorPanel, BoxLayout.PAGE_AXIS));
        panel3 = new JPanel();
        panel3.setLayout(new BoxLayout(panel3, BoxLayout.PAGE_AXIS));
        buttonPanel = new JPanel();
        buttonPanel.setLayout(new BoxLayout(buttonPanel, BoxLayout.PAGE_AXIS));

        // Initialize sensors
        for(int i = 1; i < 7; i++){
            sensors[i] = new SensorComponent("sensor"+i);
        }
        gyro = new SensorComponent("gyro");
        mode = new SensorComponent("mode");

        // Initialize decision log
        textPanel = new TextComponent();

        // Initialize parameters
        counterLimit =  new ParameterComponent("counterLimit");
        walkAfterRotationValue = new ParameterComponent("walkAfterRotationValue");
        allowedAngleError = new ParameterComponent("allowedAngleError");
        angleLimit = new ParameterComponent("angleLimit");
        distanceToSideWallLimit = new ParameterComponent("distanceToSideWallLimit");
        sensorstring = new ParameterComponent("sensorstring");
        movementstring = new ParameterComponent("movementstring");
        speed = new ParameterComponent("speed");
        distanceToWallUpperBound = new ParameterComponent("distanceToWallUpperBound");
        distanceToWallLowerBound = new ParameterComponent("distanceToWallLowerBound");
        goalBound = new ParameterComponent("goalBound");
        stopBound = new ParameterComponent("stopBound");
        sensorDelay = new ParameterComponent("sensorDelay");

        // Initialize buttons
        changeMode = new ButtonComponent("changeMode");
        init = new ButtonComponent("init");
        angels = new ButtonComponent("angles");

        // Add buttons
        buttonPanel.add(changeMode);
        buttonPanel.add(init);
        buttonPanel.add(angels);

        // Add parameters
        parameterPanel.add(counterLimit);
        parameterPanel.add(walkAfterRotationValue);
        parameterPanel.add(allowedAngleError);
        parameterPanel.add(angleLimit);
        parameterPanel.add(distanceToSideWallLimit);
        parameterPanel.add(speed);
        parameterPanel.add(distanceToWallUpperBound);
        parameterPanel.add(distanceToWallLowerBound);
        parameterPanel.add(goalBound);
        parameterPanel.add(stopBound);
        parameterPanel.add(sensorDelay);
        parameterPanel.add(sensorstring);
        parameterPanel.add(movementstring);


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
    }
}