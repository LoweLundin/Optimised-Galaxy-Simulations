# Optimised Galaxy Simulations in C

This project was part of the course High Performance Computing taken at Uppsala University during 2019. Mind that the code was produced at an earlier stage in my education and that everything might not be best practice.

The project is based on Newton's law of universal gravitation,$F=G\frac{m_1m_2}{r^2}$, basically "matter attracts gravitationally relative to weight and distance", with every dot on the display representing a star with a unique position, mass and velocity.

The project is split in two parts,

## Star-by-star

This is a "vanilla", unoptimised version, where the gravitational pull on each star is computed for each other star individually, by making use of Newton's third law, "When one body exerts a force on a second body, the second body simultaneously exerts a force equal in magnitude and opposite in direction on the first body.", we can reduce the number of computations needed per timestep to $\frac{N^2}{2}$, where N is the number of stars, but complexity is still $\mathcal{O}(N^2)$, which means our problem quickly scales in size.

## Barnes-Hut-Parallelised

In this version of the program the [Barnes-Hut Approximation algorithm](https://en.wikipedia.org/wiki/Barnes%E2%80%93Hut_simulation) for 2D problems is implemented, in which multiple stars can be approximated with a single center of mass if the quotient between the width of a cluster of stars and the distance from the body to the center of mass is smaller than a  threshhold value $\theta_{max}$, chosen by the user. The program has also been parallelised using pthreads.

## Installation
Note: This has so far only been tried out on Windows with Ubuntu Terminal.

Download the repository and compile the program
```
$ cd <WORKING-DIRECTORY>
$ git clone https://github.com/LoweLundin/Optimised--Simulations-in-C
$ cd <WORKING-DIRECTORY>/Star-by-star # For Star-by-star
$ cd <WORKING-DIRECTORY>/Barnes-Hut-Parallelised # For Barnes-Hut-Parallelised
$ make
```

## Usage

If you want graphics, make sure you have an X server running, Xming has been used in development.

For Star-by-star: 
```
$ cd <WORKING-DIRECTORY>/Star-by-star

Enter:
$ time ./galsim [number of stars in simulation] [input file to read] [number of timesteps to run] [delta t] [graphics on/off boolean]

Example: 
$ time ./galsim 2000 input_data/ellipse_N_02000.gal 1000 0.001 1
```

For Barnes-Hut-Parallelised:
```
$ cd <WORKING-DIRECTORY>/Barnes-Hut-Parallelised

Enter:
$ time ./galsim [number of stars in simulation] [input file to read] [number of timesteps to run] [delta t] [theta_max]  [graphics on/off boolean] [number of threads to run on]

Example:
$ time ./galsim 10000 input_data/ellipse_N_10000.gal 1000 0.0001 0.25 1 4
```
Note that:
\begin{itemize}
\item The "time"-command is used to deduce execution time and can be ommited. 
\item Number of stars must match N given in input file.
\item delta t is the time elapsed between each computation, smaller delta t means more exact, but slower computation.
\item The program can be run for really large $\theta_{max}$-values, but certain stars might stray "out of bounds", at which time the program is stopped.
\item Running with graphics on means longer running times and should not be used when trying to push numbers.
\item The program is runnable for any number of threads except zero, but the program can only be expected to run faster up until the number of threads is twice that of the number of cores. 
\item If one stops caring about approximations, the model can be run quite effectively with Barnes-Hut, 
try for example:  time ./galsim 20000 input_data/ellipse_N_20000.gal 1000 0.001 1 1 4
\end{itemize}

## Correctness of simulations

To control the "correctness" of the simulations, "compare_gal_files" can be used.

To use, step into the correct directory, then compile and run
```bash
$ cd <WORKING-DIRECTORY>/Star-by-star/compare_gal_files # For Star-by-star
$ cd <WORKING-DIRECTORY>/Barnes-Hut-Parallelised/compare_gal_files # For Barnes-Hut-Parallelised

$ gcc -o compare_gal_files compare_gal_files.c -lm

To run:
$ ./compare_gal_files [number of stars in files] [reference output data file] [result from last simulation, stored in "result.gal"] 

Example:
$ ./compare_gal_files 3000 ref_output_data/ellipse_N_03000_after200steps.gal ../result.gal
```

The "ref_output_data"-files have been generated using Star-by-star, with epsilon = 0.15 and dT = 0.0001, number of stars and timesteps given in filename. If comparing result from Star-by-star with equal parameters, one should expect error to be 0.
