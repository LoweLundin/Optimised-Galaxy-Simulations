# Optimised Galaxy Simulations

The project is based on Newton's law of universal gravitation,<img src="/tex/ebe1a6a00f5417138a18fd859a6bb504.svg?invert_in_darkmode&sanitize=true&sanitize=true" align=middle width=85.82971649999999pt height=23.388043799999995pt/>, basically "matter attracts gravitationally relative to weight and distance", with every dot on the display representing a star with a unique position, mass and velocity. At t=0, all stars are initialised as an ellipse, the spiral galaxy isn't hard-coded, but an emergent property of gravitation. 

<p align="center">
  <img width="500" height="500" src=galsim10000.gif>

</p>
<p align="center">Barnes-Hut-Parallelised run with 10000 stars, <img src="/tex/7e9fe18dc67705c858c077c5ee292ab4.svg?invert_in_darkmode&sanitize=true" align=middle width=13.69867124999999pt height=22.465723500000017pt/>t = 0.0001 and <img src="/tex/20d4bcfec3faaaae8404a53c8fc76fb2.svg?invert_in_darkmode&sanitize=true" align=middle width=33.96649739999999pt height=22.831056599999986pt/> = 0.9. <br>
Note that this is not pre-rendered, but computed in realtime. The somewhat "laggy" appearance and low framerate compared to actual run is due to GIF-recording limitations, it's actually much smoother, promise. 
</p>

The project is split in two parts, Star-by-star and Barnes-Hut-Parallelised.

## Star-by-star

This is a "vanilla", unoptimised version, where the gravitational pull on each star is computed for each other star individually, by making use of Newton's third law, 

> When one body exerts a force on a second body, the second body simultaneously exerts a force equal in magnitude and opposite in direction on the first body.

we can reduce the number of computations needed per timestep to <img src="/tex/f97c1a43186c0257a4914ddce10d9cda.svg?invert_in_darkmode&sanitize=true&sanitize=true" align=middle width=18.061748099999996pt height=33.45973289999998pt/>, where N is the number of stars, but complexity is still <img src="/tex/8e90dbe2d3ca28b3ad0012cb03e7ead6.svg?invert_in_darkmode&sanitize=true&sanitize=true" align=middle width=48.70330244999999pt height=26.76175259999998pt/>, which means our problem quickly scales in size.

## Barnes-Hut-Parallelised

In this version of the program the [Barnes-Hut approximation algorithm](https://en.wikipedia.org/wiki/Barnes%E2%80%93Hut_simulation) for 2D problems is implemented, in which multiple stars can be approximated with a single center of mass if the quotient between the width of a cluster of stars and the distance from the body to the center of mass is smaller than a  threshhold value <img src="/tex/20d4bcfec3faaaae8404a53c8fc76fb2.svg?invert_in_darkmode&sanitize=true&sanitize=true" align=middle width=33.96649739999999pt height=22.831056599999986pt/>, chosen by the user. This means the Barnes-Hut implementation can run with complexity <img src="/tex/5e99fdb26ddd5a0574d81f3326828c09.svg?invert_in_darkmode&sanitize=true" align=middle width=77.95556339999999pt height=24.65753399999998pt/>.

The program has also been parallelised using pthreads.

## Installation
Note: This has so far only been tried out on Windows with Ubuntu Terminal.

To install:
```
Step into the working directory:
$ cd <WORKING-DIRECTORY>

Download the repository:
$ git clone https://github.com/LoweLundin/Optimised-Galaxy-Simulations

Step into desired directory:
$ cd Optimised-Galaxy-Simulations/Star-by-star # For Star-by-star
$ cd Optimised-Galaxy-Simulations/Barnes-Hut-Parallelised # For Barnes-Hut-Parallelised

Compile the program:
$ make
```

## Usage

If you want graphics, make sure you have an X server running, Xming has been used in development.

For Star-by-star: 
```
Step into directory:
$ cd <WORKING-DIRECTORY>/Optimised-Galaxy-Simulations/Star-by-star

Enter:
$ time ./galsim [number of stars in simulation] [input file to read] [number of timesteps to run] [delta t] [graphics on/off boolean]

Example: 
$ time ./galsim 2000 input_data/ellipse_N_02000.gal 1000 0.001 1
```

For Barnes-Hut-Parallelised:
```
Step into the directory:
$ cd <WORKING-DIRECTORY>/Optimised-Galaxy-Simulations/Barnes-Hut-Parallelised

Enter:
$ time ./galsim [number of stars in simulation] [input file to read] [number of timesteps to run] [delta t] [theta_max]  [graphics on/off boolean] [number of threads to run on]

Example:
$ time ./galsim 10000 input_data/ellipse_N_10000.gal 1000 0.0001 0.25 1 4
```
Try playing around with different parameters.

Note that:
<p align="center"><img src="/tex/59ddc08a56b7bd9c226ed28bca97979a.svg?invert_in_darkmode&sanitize=true&sanitize=true" align=middle width=676.8955676999999pt height=327.03196679999996pt/></p>

## Correctness of simulations

To control the "correctness" of the simulations, "compare_gal_files" can be used.

To use:

```
Step into the directory:
$ cd <WORKING-DIRECTORY>/Optimised-Galaxy-Simulations/Star-by-star/compare_gal_files # For Star-by-star
$ cd <WORKING-DIRECTORY>/Optimised-Galaxy-Simulations/Barnes-Hut-Parallelised/compare_gal_files # For Barnes-Hut-Parallelised

Compile:
$ gcc -o compare_gal_files compare_gal_files.c -lm

To run:
$ ./compare_gal_files [number of stars in files] [reference output data file] [result from last simulation, stored in "result.gal"] 

Example:
$ ./compare_gal_files 3000 ref_output_data/ellipse_N_03000_after200steps.gal ../result.gal
```

The "ref_output_data"-files have been generated using Star-by-star, with epsilon = 0.15 and dT = 0.0001, number of stars and timesteps given in filename. If comparing reference output data files to result from Star-by-star or Barnes-Hut-Parallelised with theta_max = 0, error is expected to be 0.
