import javax.swing.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

/**
 * Created by danielfalk on 11/25/14.
 */
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
