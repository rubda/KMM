import javax.swing.*;
import javax.swing.text.DefaultCaret;
import java.awt.*;

public class TextComponent extends JPanel {

    JTextArea decisions;
    JScrollPane scrollPane;

    public TextComponent(){
        decisions = new JTextArea();
        decisions.setLineWrap(true);

        scrollPane = new JScrollPane(decisions);
        scrollPane.setPreferredSize(new Dimension(250, 600));

        // Auto scroll
        DefaultCaret caret = (DefaultCaret)decisions.getCaret();
        caret.setUpdatePolicy(DefaultCaret.ALWAYS_UPDATE);

        add(scrollPane);
    }

    public void add(String string){
        decisions.append(string);
        decisions.append("\n");
    }
}
