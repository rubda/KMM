import javax.swing.*;
import java.awt.event.ActionEvent;
import java.security.Key;

public class MainComponent extends JComponent {

    Boolean rr, rl, f, b;

    public MainComponent(){
        rr = false;
        rl = false;
        f = false;
        b = false;
        addBindings();
        setFocusable(true);
    }

    private void addBindings(){

        InputMap map = getInputMap(JComponent.WHEN_IN_FOCUSED_WINDOW);


        map.put(KeyStroke.getKeyStroke("pressed LEFT"), "rotateLeft");
        map.put(KeyStroke.getKeyStroke("pressed RIGHT"), "rotateRight");
        map.put(KeyStroke.getKeyStroke("pressed UP"), "walkForward");
        map.put(KeyStroke.getKeyStroke("pressed DOWN"), "walkBackwards");
        map.put(KeyStroke.getKeyStroke("pressed 7"), "strafeLeftForward");
        map.put(KeyStroke.getKeyStroke("pressed 9"), "strafeRightForward");
        map.put(KeyStroke.getKeyStroke("pressed R"), "reset");

        map.put(KeyStroke.getKeyStroke("released 7"), "stopStrafeLeftForward");
        map.put(KeyStroke.getKeyStroke("released 9"), "stopStrafeRightForward");
        map.put(KeyStroke.getKeyStroke("released LEFT"), "stopRotateLeft");
        map.put(KeyStroke.getKeyStroke("released RIGHT"), "stopRotateRight");
        map.put(KeyStroke.getKeyStroke("released UP"), "stopWalkingForward");
        map.put(KeyStroke.getKeyStroke("released DOWN"), "stopWalkingBackwards");


        Action reset = new AbstractAction() {
            @Override
            public void actionPerformed(ActionEvent actionEvent) {
                System.out.println("reset");
                requestFocus();
            }
        };

        Action rotateLeft = new AbstractAction(){
            public void actionPerformed(final ActionEvent e) {
                rl = true;
                if(f && !rr && !b){
                    Communication.sendAction("#turn:l;");
                }
                else{
                    Communication.sendAction("#rotate:l;");
                }
            }
        };
        Action rotateRight = new AbstractAction(){
            public void actionPerformed(final ActionEvent e) {
                rr = true;
                if(f && !rl && !b){
                    Communication.sendAction("#turn:r;");
                }
                else{
                Communication.sendAction("#rotate:r;");
                }
            }
        };
        Action walkForward = new AbstractAction(){
            public void actionPerformed(final ActionEvent e) {
                f = true;
                if(rl && !rr && !b){
                Communication.sendAction("#turn:l;");
                }
                else if(!rl && rr && !b){
                    Communication.sendAction("#turn:r;");
                }
                else{
                    Communication.sendAction("#walk:f:0;");

                }
            }
        };

        Action strafeLeftForward = new AbstractAction() {
            @Override
            public void actionPerformed(final ActionEvent e) {
                Communication.sendAction("#walk:f:45;");
            }
        };
        Action strafeRightForward = new AbstractAction() {
            @Override
            public void actionPerformed(final ActionEvent e) {
                Communication.sendAction("#walk:f:315;");
            }
        };
        Action walkBackwards = new AbstractAction(){
            public void actionPerformed(final ActionEvent e) {
                b = true;
                Communication.sendAction("#walk:b;");
            }
        };
        Action stopRotateLeft = new AbstractAction()
        {
            public void actionPerformed(final ActionEvent e) {
                rl = false;
                if (!f && !b && !rr) {
                    Communication.sendAction("#stop:after;");
                }
            }
        };
        Action stopRotateRight = new AbstractAction()
        {
            public void actionPerformed(final ActionEvent e) {
                 rr = false;
                if (!f && !b && !rl) {
                    Communication.sendAction("#stop:after;");
                }
            }
        };
        Action stopWalkingForward = new AbstractAction()
        {
            public void actionPerformed(final ActionEvent e) {
                f = false;
                if(!b && !rl && !rr){
                    Communication.sendAction("#stop:after;");
                }
            }
        };
        Action stopWalkingBackwards = new AbstractAction()
        {
            public void actionPerformed(final ActionEvent e) {
                b = false;
                if(!f && !rl && !rr){
                    Communication.sendAction("#stop:after;");
                }
            }
        };
        Action stopStrafeLeftForward = new AbstractAction() {
            @Override
            public void actionPerformed(final ActionEvent e) {
                Communication.sendAction("#stop:after;");
            }
        };
        Action stopStrafeRightForward = new AbstractAction() {
            @Override
            public void actionPerformed(final ActionEvent e) {
                Communication.sendAction("#stop:after;");
            }
        };
        getActionMap().put("reset", reset);
        getActionMap().put("rotateLeft", rotateLeft);
        getActionMap().put("rotateRight", rotateRight);
        getActionMap().put("walkForward", walkForward);
        getActionMap().put("walkBackwards", walkBackwards);
        getActionMap().put("strafeLeftForward", strafeLeftForward);
        getActionMap().put("strafeRightForward", strafeRightForward);
        getActionMap().put("stopRotateLeft", stopRotateLeft);
        getActionMap().put("stopRotateRight", stopRotateRight);
        getActionMap().put("stopWalkingForward", stopWalkingForward);
        getActionMap().put("stopWalkingBackwards", stopWalkingBackwards);
        getActionMap().put("stopStrafeLeftForward", stopStrafeLeftForward);
        getActionMap().put("stopStrafeRightForward", stopStrafeRightForward);

    }
}
