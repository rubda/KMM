import javax.swing.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

/**
 * Created by danielfalk on 11/3/14.
 */
public class ParameterComponent extends JPanel {

JTextField field = new JTextField(10);

    public ParameterComponent(String name){
        JLabel jlabel = new JLabel(name);
        add(jlabel);

        final String[] textString = new String[2];
        textString[0] = name;
        field.addActionListener(new ActionListener()
        {
            public void actionPerformed(ActionEvent e)
            {
                textString[1] = field.getText();
                Communication.sendParameter(textString);
            }
        });
        add(field);

    }
}
