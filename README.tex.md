# Optimised Galaxy Simulations in C

This project was part of the course High Performance Computing taken at Uppsala University during 2019. Mind that the code was produced at an earlier stage in my education and that everything might not be best practice.

The project is based on Newton's law of universal gravitation,$F=G\frac{m_1m_2}{r^2}$, basically "matter attracts gravitationally relative to weight and distance", with every dot on the display representing a planet with a unique position, mass and velocity.

The project is split in two parts,

## Star-by-star

A "vanilla", unoptimised version, where the gravitational pull on each planet is computed for each other planet individually, by making use of Newton's third law, "When one body exerts a force on a second body, the second body simultaneously exerts a force equal in magnitude and opposite in direction on the first body.", we can reduce the number of computations needed per timestep to ,$\frac{N^2}{2}$, , where N is the number of stars, but complexity is still $\mathcal{O}(N^2)$, which means our problem quickly scales in size.

## Barnes-Hut-Parallelised

Where the [Barnes-Hut Approximation algorithm](https://en.wikipedia.org/wiki/Barnes%E2%80%93Hut_simulation) for 2D problems is implemented, in which multiple planets can be approximated with a single center of mass if the quotient between the width of a cluster of planets and the distance from the body to the center of mass is smaller than a  threshhold value $\theta_{max}$, chosen by the user. 

## Installation

'''bash
Note: This has so far only been tried out on Windows with Ubuntu Terminal.

Clone into the repository git clone https://github.com/LoweLundin/Optimised-galaxy-simulations-in-C

Open the download diretory in terminal

Type "make"
'''

## Usage

'''bash
If you want graphics, make sure you have an X server running, Xming has been used in development.

step into the directory of whichever version you want to run:

For Star-by-star, enter ./galsim [number of stars in simulation] [input file to read] [number of timesteps to run] [delta t] [graphics on/off boolean]


For Barnes-Hut-Parallelised, enter ./galsim [number of stars in simulation] [input file to read] [number of timesteps to run] [delta t] [$\theta_{max}$]  [graphics on/off boolean] [number of threads to run on]
 
Note that:
\begin{itemize}
\item Number of stars must match N given in input file.
\item delta t is the time elapsed between each computation, smaller delta t means more exact, but slower computation
\item The program can be run for really large theta max, but certain stars might stray "out of bounds" at which time, the program is stopped
\item Running with graphics on means longer running times and should not be used when trying to push numbers
\item The program is runnable for any number of threads except zero, but the program can only be expected to run faster up until the number of threads is twice that of the number of cores. 
\end{itemize}


'''
