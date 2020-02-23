# Optimised Galaxy Simulations in C

This project was part of the course High Performance Computing taken at Uppsala University during 2019. Mind that the code was produced at an earlier stage in my education and that everything might not be best practice.

The project is based on Newton's law of universal gravitation,<img src="/tex/ebe1a6a00f5417138a18fd859a6bb504.svg?invert_in_darkmode&sanitize=true" align=middle width=85.82971649999999pt height=23.388043799999995pt/>, basically "matter attracts gravitationally relative to weight and distance", with every dot on the display representing a star with a unique position, mass and velocity.

The project is split in two parts,

## Star-by-star

This is a "vanilla", unoptimised version, where the gravitational pull on each star is computed for each other star individually, by making use of Newton's third law, 

> "When one body exerts a force on a second body, the second body simultaneously exerts a force equal in magnitude and opposite in direction on the first body.", 

we can reduce the number of computations needed per timestep to <img src="/tex/f97c1a43186c0257a4914ddce10d9cda.svg?invert_in_darkmode&sanitize=true" align=middle width=18.061748099999996pt height=33.45973289999998pt/>, where N is the number of stars, but complexity is still <img src="/tex/8e90dbe2d3ca28b3ad0012cb03e7ead6.svg?invert_in_darkmode&sanitize=true" align=middle width=48.70330244999999pt height=26.76175259999998pt/>, which means our problem quickly scales in size.

## Barnes-Hut-Parallelised

In this version of the program the [Barnes-Hut Approximation algorithm](https://en.wikipedia.org/wiki/Barnes%E2%80%93Hut_simulation) for 2D problems is implemented, in which multiple stars can be approximated with a single center of mass if the quotient between the width of a cluster of stars and the distance from the body to the center of mass is smaller than a  threshhold value <img src="/tex/20d4bcfec3faaaae8404a53c8fc76fb2.svg?invert_in_darkmode&sanitize=true" align=middle width=33.96649739999999pt height=22.831056599999986pt/>, chosen by the user. The program has also been parallelised using pthreads.

## Installation
Note: This has so far only been tried out on Windows with Ubuntu Terminal.

Download the repository and compile the program
```
<img src="/tex/573335fd53c0c98d56bdd0aefccf62f6.svg?invert_in_darkmode&sanitize=true" align=middle width=281.90691869999995pt height=22.831056599999986pt/> git clone https://github.com/LoweLundin/Optimised--Simulations-in-C

<img src="/tex/4a9cdd6f97280d0448172d53ff8ca3ec.svg?invert_in_darkmode&sanitize=true" align=middle width=562.61847675pt height=24.65753399999998pt/> cd <WORKING-DIRECTORY>/Barnes-Hut-Parallelised # For Barnes-Hut-Parallelised

<img src="/tex/84f466ab274fa967be82f75a9ed7f0b2.svg?invert_in_darkmode&sanitize=true" align=middle width=700.27459095pt height=716.5296864pt/>\theta_{max}$-values, but certain stars might stray "out of bounds", at which time the program is stopped.
\item Running with graphics on means longer running times and should not be used when trying to push numbers.
\item The program is runnable for any number of threads except zero, but the program can only be expected to run faster up until the number of threads is twice that of the number of cores. 
\item If one stops caring about approximations, the model can be run quite effectively with Barnes-Hut, 
try for example:  time ./galsim 20000 input_data/ellipse_N_20000.gal 1000 0.001 1 1 4
\end{itemize}

## Correctness of simulations

To control the "correctness" of the simulations, "compare_gal_files" can be used.

To use, step into the correct directory, then compile and run
```bash
cd <WORKING-DIRECTORY>/Star-by-star/compare_gal_files # For Star-by-star
cd <WORKING-DIRECTORY>/Barnes-Hut-Parallelised/compare_gal_files # For Barnes-Hut-Parallelised

gcc -o compare_gal_files compare_gal_files.c -lm

To run:
./compare_gal_files [number of stars in files] [reference output data file] [result from last simulation, stored in "result.gal"] 

Example:
./compare_gal_files 3000 ref_output_data/ellipse_N_03000_after200steps.gal ../result.gal
```

The "ref_output_data"-files have been generated using Star-by-star, with epsilon = 0.15 and dT = 0.0001, number of stars and timesteps given in filename. If comparing result from Star-by-star with equal parameters, one should expect error to be 0.
