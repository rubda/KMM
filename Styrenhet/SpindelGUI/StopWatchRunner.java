import java.awt.event.*;
import javax.swing.*;

public class StopWatchRunner extends JLabel implements ActionListener {

    private long startTime;   // Start time of stopwatch. (Time is measured in milliseconds.)
    private boolean running;  // True when the stopwatch is running.
    private Timer timer;  // A timer that will generate events while the stopwatch is running

    public StopWatchRunner() {
        super("timer", JLabel.CENTER);
    }

    public void actionPerformed(ActionEvent evt) {
        /* This will be called when an event from the
           timer is received.  It just sets the stopwatch
           to show the amount of time that it has been running.
           Time is rounded down to the nearest second.  */
        long time = (System.currentTimeMillis()-startTime) / 1000;
        setText("Running:  " + time + " seconds");
    }

    // Record the time and start the stopwatch.
    public void start() {
        if (running == false) {
            running = true;
            startTime = System.currentTimeMillis();
            setText("Running:  0 seconds");
            if (timer == null) {
                timer = new Timer(100,this);
                timer.start();
            }
            else
                timer.restart();
        }
    }

    // Stop the stopwatch and print the time
    public void stop(){
        timer.stop();
        running = false;
        long endTime = System.currentTimeMillis();
        double seconds = (endTime-startTime) / 1000.0;
        setText("Time: " + seconds + " sec.");
    }
}

