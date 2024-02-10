import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;
import java.io.IOException;

public class Compare {

    public static void main(String[] args) {
        int len = args.length;
        if (len < 2) {
            System.out.println("Usage: [FILES]");
            return;
        }
        BufferedReader readers[] = new BufferedReader[len];
        String lines[] = new String[len];
        for (int i = 0; i < len; i++) {
            try {
                File f = new File(args[i]);
                readers[i] = new BufferedReader(new FileReader(f));
            } catch(IOException ex) {
                ex.printStackTrace();
            }
        }
        try {
            int lineDepth = 0;
            boolean shouldExit = false;
            while (!shouldExit) {
                for (int i = 0; i < len; i++) {
                    lines[i] = readers[i].readLine();
                    if (lines[i] == null) {
                        shouldExit = true;
                        break;
                    }
                }
                for (int i = 0; i < len - 1; i++) {
                    String s1 = lines[i], s2 = lines[i + 1];
                    if (s1 == null || s2 == null) {
                        break;
                    }
                    if (!s1.equals(s2)) {
                        System.out.printf("%s != %s on line %d\n", args[i], args[i + 1], lineDepth);
                        System.out.printf("\t%s)\t%s\n\t%s)\t%s\n", args[i], s1, args[i + 1], s2);
                    }
                }
                lineDepth++;
            }
            System.out.printf("Analyzed %d lines\n", lineDepth);
        } catch(IOException ex) {
            ex.printStackTrace();
        }
    }

}