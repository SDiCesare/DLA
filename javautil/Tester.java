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
        int sizes[] = {100, 1000, 10000, 1000000};
        int particles[] = {256, 1024, 8192, 32768};
        int steps[] = {100, 1000, 10000, 1000000};
        for (int size : sizes) {
            for (int particle : particles) {
                for (int step : steps) {
                    tests.add(new Test(size, size, particle, step));
                }
            }
        }
        System.out.printf("#!/bin/sh\n" + 
            "echo \"Clearing Previous Tests\"\n"+
            "rm -fr ./out\n" +
            "mkdir ./out\n" +
            "echo \"=======================================\"\n");
        for (int i = 0; i < tests.size(); i++) {
            Test test = tests.get(i);
            // ./PATH/main WIDTH HEIGHT PARTICLES STEPS CENTER_X CENTER_Y OUT_MAP > OUT.TXT
            System.out.printf("echo \"Test %d\"\n", i);
            System.out.printf("echo \"Params: %d %d %d %d 99 99\"\n", test.width, test.height, test.particles, test.steps);
            System.out.printf("echo \"Serial Simulation\"\n");
            System.out.printf("./serial/main %d %d %d %d 99 99 ./out/serial_map_%d.txt > ./out/serial_%d.txt\n", test.width, test.height, test.particles, test.steps, i, i);
            System.out.printf("echo \"OpenMP Simulation\"\n");
            System.out.printf("./openmp/main %d %d %d %d 99 99 ./out/openmp_map_%d.txt > ./out/openmp_%d.txt\n", test.width, test.height, test.particles, test.steps, i, i);
            System.out.printf("echo \"Cuda Simulation\"\n");
            System.out.printf("./cuda/main %d %d %d %d 99 99 ./out/cuda_map_%d.txt > ./out/cuda_%d.txt\n", test.width, test.height, test.particles, test.steps, i, i);
            System.out.printf("echo \"=======================================\"\n");
        }
    }

} 