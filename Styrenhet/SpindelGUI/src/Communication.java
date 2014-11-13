public class Communication {

    static String[] parts;

    public Communication(String message){
    }

    // Send parameters
    public static void sendParameter(String[] message){
        System.out.println("#param:"+message[0]+":"+message[1]+";");
    }
    // Send action
    public static void sendAction(String message){
        System.out.println(message);
    }

    // Receive and interpret a message
    public static void receive(String message){

        message = message.substring(1, message.length()-1);
        parts = message.split(":");
        if(parts[0].equals("distance")){
            FrameWork.sensors[Integer.parseInt(parts[1])].setData(parts[2]);
            FrameWork.textPanel.add("Sensor" + Integer.parseInt(parts[1]) + ": " + Integer.parseInt(parts[2]));
        }
        else if(parts[0].equals("mode")){
            FrameWork.mode.setData(parts[1]);
            FrameWork.textPanel.add("Mode: "+parts[1]);
        }
        else if(parts[0].equals("time")){
            if(parts[1].equals("1")){
                FrameWork.watch.start();
            }
            else if(parts[1].equals("0")){
                FrameWork.watch.stop();
            }
        }
        else if(parts[0].equals("action")){
            FrameWork.textPanel.add("Styrbeslut: " + parts[1]);
        }
        else{
            System.out.println("FEL!!!!");
        }


    }


}


