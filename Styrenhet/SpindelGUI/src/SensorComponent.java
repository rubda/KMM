import javax.swing.*;
import javax.swing.border.EmptyBorder;

public class SensorComponent extends JPanel {

    String data = "";
    String name;
    JTextArea textArea;

    public SensorComponent(String name){
        setLayout(new BoxLayout(this, BoxLayout.PAGE_AXIS));
        setBorder(new EmptyBorder(5,5,5,5));
        this.name = name;

        textArea = new JTextArea();
        textArea.setText(name + ":\n" + data);
        textArea.setEditable(false);
        add(textArea);

    }

    public void setData(String data){
        textArea.setText(name+":\n"+data);

    }

}


