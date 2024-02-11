# DLA
Diffusion-limited aggregation in C, written in a serial version and in CUDA and OpenMP for performance.

# USAGE
Build all tree binary by running  
`make`  
And then run a version by typing  
`./bin/VERSION_dla WIDTH HEIGHT PARTICLES STEPS CENTER_X CENTER_Y out_map.txt`  
With VERSION being one of [serial, openmp, cuda].  
WIDTH and HEIGHT are the integers representing the size of the DLA grid.  
PARTICLES is an integer representing the number of particles for the simulation.  
STEPS is an integer representing the number of steps the simulation should do.  
CENTER_X and CENTER_Y are integers representing the posizion (x, y) of the first crystal in the simulation.  
out_map.txt is an optional file in which the generated crystal is saved. Default is crystal.txt  

# OUTPUT
The output of the simulation is a txt file, and can be transformed into an image by running  
`java javautil/Printe.java WIDTH HEIGHT crystal.txt`  
The result is a file crystal.png.  

# DLA
![alt text](crystal.png)
