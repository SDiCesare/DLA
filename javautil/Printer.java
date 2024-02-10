import java.io.*; 
import java.awt.image.BufferedImage;
import java.awt.Color;
import javax.imageio.ImageIO;

public class Printer {

    public static void main(String[] args) {
        if (args.length < 3) {
            System.out.println("Usage: WIDTH HEIGHT [FILES]");
            return;
        }
        int width = Integer.parseInt(args[0]);
        int height = Integer.parseInt(args[1]);
        for (int i = 2; i < args.length; i++) {
            String file_name = args[i];
            if (!file_name.endsWith(".txt")) {
                System.out.println("File " + file_name + " invalid!");
                continue;
            }
            try {
                File file = new File(file_name);
                if (!file.exists()) {
                    System.out.println("File " + file_name + " does not exists!");
                    continue;
                }
                FileReader fr = new FileReader(file);
                BufferedReader reader = new BufferedReader(fr);
                BufferedImage image = new BufferedImage(width, height, BufferedImage.TYPE_INT_RGB);
                String ln = reader.readLine();
                int y = 0;
                while (ln != null) {
                    String[] data = ln.split(" ");
                    ln = reader.readLine();
                    if (data.length != width) {
                        // Skip non data line (aka info lines)
                        continue;
                    }
                    for (int x = 0; x < data.length; x++) {
                        int n = Integer.parseInt(data[x]);
                        if (n == 0) {
                            // No Crystal Here
                            image.setRGB(x, y, new Color(200, 0, 0).getRGB());
                        } else {
                            n = Math.min(25 * n, 255); // Arbitrary scale for particle density in eah cell
                            image.setRGB(x, y, new Color(n, n, n).getRGB());
                        }
                    }
                    y++; // Go to next "layer"
                    if (y >= height) { // Security Layer
                        break;
                    }
                }
                reader.close();
                ImageIO.write(image, "PNG", new File(file_name.replace(".txt", ".png")));
            } catch(IOException ex) {
                System.out.println("Error While Processing " + file_name + "\n\t" + ex.getMessage());
            }
        }
    }

}