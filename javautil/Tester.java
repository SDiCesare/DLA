import java.util.ArrayList;

public class Tester {

    private static class Test {
        private int width;
        private int height;
        private int particles;
        private int steps;

        private Test(int width, int height, int particles, int steps) {
            this.width = width;
            this.height = height;
            this.particles = particles;
            this.steps = steps;
        }
    }

    public static void main(String[] args) {
        ArrayList<Test> tests = new ArrayList<>();
        int sizes[] = {100, 1000, 10000, 1000000}; // 1000000
        int particles[] = {32768};
        int steps[] = {10000};
        String outDir = "./sizes";
        for (int size : sizes) {
            for (int particle : particles) {
                for (int step : steps) {
                    tests.add(new Test(size, size, particle, step));
                }
            }
        }
        System.out.printf("#!/bin/sh\n" + 
            "echo \"Clearing Previous Tests\"\n"+
            "rm -fr %s\n" +
            "mkdir %s\n" +
            "echo \"=======================================\"\n", outDir, outDir);
        for (int i = 0; i < tests.size(); i++) {
            Test test = tests.get(i);
            // ./PATH/main WIDTH HEIGHT PARTICLES STEPS CENTER_X CENTER_Y OUT_MAP > OUT.TXT
            System.out.printf("echo \"Test %d\"\n", i);
            System.out.printf("echo \"Params: %d %d %d %d 0 0\"\n", test.width, test.height, test.particles, test.steps);
            for (String name : new String[]{"serial", "openmp", "cuda"}) {
                System.out.printf("echo \"%s simulation\"\n", name);
                String dir = "./bin/" + name + "_dla";
                System.out.printf("%s %d %d %d %d 0 0 %s/%s_map_%d.txt > %s/%s_%d.txt\n", dir,
                    test.width, test.height, test.particles, test.steps,
                    outDir, name, i, outDir, name, i);
            }
            System.out.printf("echo \"=======================================\"\n");
        }
    }

} 