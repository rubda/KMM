import javax.swing.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

public class ButtonComponent extends JPanel {

    JButton button = new JButton();

    public ButtonComponent(String name){
        button.setText(name);

        final String[] textString = new String[2];
        textString[0] = name;
        textString[1] = ""; //inget
        button.addActionListener(new ActionListener()
        {
            public void actionPerformed(ActionEvent e)
            {
                Communication.sendParameter(textString);
            }
        });
        add(button);
    }
}
