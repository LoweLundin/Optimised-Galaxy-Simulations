# Optimised Galaxy Simulations in C

This project was part of the course High Performance Computing taken at Uppsala University during 2019. Mind that the code was produced at an earlier stage in my education and that everything might not be best practice.

The project is based on Newton's law of universal gravitation,<img src="/tex/ebe1a6a00f5417138a18fd859a6bb504.svg?invert_in_darkmode&sanitize=true&sanitize=true" align=middle width=85.82971649999999pt height=23.388043799999995pt/>, basically "matter attracts gravitationally relative to weight and distance", with every dot on the display representing a star with a unique position, mass and velocity. At t=0 all stars are initialised in the structure of an ellipse.

The project is split in two parts, Star-by-star and Barnes-Hut-Parallelised.

<p align="center">
  <img width="460" height="460" src=galsim.gif>

</p>
<p align="center">Barnes-Hut-Parallelised run with 10000 stars, <img src="/tex/38f1e2a089e53d5c990a82f284948953.svg?invert_in_darkmode&sanitize=true" align=middle width=7.928075099999989pt height=22.831056599999986pt/>t = 0.0001 and <img src="/tex/20d4bcfec3faaaae8404a53c8fc76fb2.svg?invert_in_darkmode&sanitize=true" align=middle width=33.96649739999999pt height=22.831056599999986pt/> = 0.9. <br>
  Note that "laggy" appearance and low framerate compared to actual run is due to GIF-format limitations.
</p>


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
<img src="/tex/1bfdc96c95b4e4034f008e2a64b89f07.svg?invert_in_darkmode&sanitize=true" align=middle width=281.90691869999995pt height=45.84475500000001pt/> git clone https://github.com/LoweLundin/Optimised-Galaxy-Simulations-in-C

Step into desired directory:
<img src="/tex/ea408c454b1cacdd159122ee124cb5cd.svg?invert_in_darkmode&sanitize=true" align=middle width=622.22202075pt height=24.65753399999998pt/> cd Optimised-Galaxy-Simulations-in-C/Barnes-Hut-Parallelised # For Barnes-Hut-Parallelised

Compile the program:
make
```

## Usage

If you want graphics, make sure you have an X server running, Xming has been used in development.

For Star-by-star: 
```
Step into directory:
<img src="/tex/5967eaf630e4f128e46e72c1fcaea590.svg?invert_in_darkmode&sanitize=true" align=middle width=700.274553pt height=78.90410880000002pt/> time ./galsim [number of stars in simulation] [input file to read] [number of timesteps to run] [delta t] [graphics on/off boolean]

Example: 
<img src="/tex/a75567dfbd60ff0899aabafb162a802e.svg?invert_in_darkmode&sanitize=true" align=middle width=426.20635695000004pt height=45.84475500000001pt/> cd <WORKING-DIRECTORY>/Optimised-Galaxy-Simulations-in-C/Barnes-Hut-Parallelised

Enter:
<img src="/tex/4c17045ed4ab971ee384a8321dab7050.svg?invert_in_darkmode&sanitize=true" align=middle width=1139.3182646999999pt height=45.84475500000001pt/> time ./galsim 10000 input_data/ellipse_N_10000.gal 1000 0.0001 0.25 1 4
```
Try playing around with different parameters.

Note that:
<p align="center"><img src="/tex/59ddc08a56b7bd9c226ed28bca97979a.svg?invert_in_darkmode&sanitize=true&sanitize=true" align=middle width=676.8955676999999pt height=327.03196679999996pt/></p>

## Correctness of simulations

To control the "correctness" of the simulations, "compare_gal_files" can be used.

To use, 

```
Step into the directory:
<img src="/tex/7357eabfe66ee6a87f6c864d1aba3be2.svg?invert_in_darkmode&sanitize=true" align=middle width=700.274553pt height=48.85840080000001pt/> cd <WORKING-DIRECTORY>/Optimised-Galaxy-Simulations-in-C/Barnes-Hut-Parallelised/compare_gal_files # For Barnes-Hut-Parallelised

Compile:
<img src="/tex/96f8f1837059c22219b8f43ddee9e9c6.svg?invert_in_darkmode&sanitize=true" align=middle width=338.18313044999996pt height=39.45205440000001pt/> ./compare_gal_files [number of stars in files] [reference output data file] [result from last simulation, stored in "result.gal"] 

Example:
<img src="/tex/06cfbf928c65e0e34460617325676837.svg?invert_in_darkmode&sanitize=true" align=middle width=821.08672965pt height=124.74886710000001pt/>\theta_{max}$ = 0, one should expect error to be 0.
