import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

public class Timer implements ActionListener
{
    private long startTime;   // Start time of stopwatch.  (Time is measured in milliseconds.)

    private boolean running;  // True when the stopwatch is running.

    private javax.swing.Timer timer;  // A timer that will generate events while the stopwatch is running

    public void actionPerformed(ActionEvent evt) {
        long time = (System.currentTimeMillis()-startTime);
        if(time>1000){
            Main.running.compareAndSet(true, false);
            stop();
        }
    }

    // Record the time and start the stopwatch.
    public void start() {
        if (running == false) {
            running = true;
            startTime = System.currentTimeMillis();
            if (timer == null) {
                timer = new javax.swing.Timer(100,this);
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
    }
}
