# Optimised Galaxy Simulations in C

This project was part of the course High Performance Computing taken at Uppsala University during 2019. Mind that the code was produced at an earlier stage in my education and that everything might not be best practice.

The project is based on Newton's law of universal gravitation,<img src="/tex/ebe1a6a00f5417138a18fd859a6bb504.svg?invert_in_darkmode&sanitize=true" align=middle width=85.82971649999999pt height=23.388043799999995pt/>, with every dot on the display representing a planet with a unique position, mass and velocity.

The project is split in two parts:

Star-by-star, a "vanilla", unoptimised version, where the gravitational pull on each planet is computed for each other planet individually, by making use of Newton's third law, 

we can reduce the number of computations needed per timestep to ,<img src="/tex/f97c1a43186c0257a4914ddce10d9cda.svg?invert_in_darkmode&sanitize=true" align=middle width=18.061748099999996pt height=33.45973289999998pt/>, , where N is the number of stars, but complexity is still <img src="/tex/8e90dbe2d3ca28b3ad0012cb03e7ead6.svg?invert_in_darkmode&sanitize=true" align=middle width=48.70330244999999pt height=26.76175259999998pt/>
 
Barnes-Hut-Pthreads, where the [Barnes-Hut Approximation algorithm](https://en.wikipedia.org/wiki/Barnes%E2%80%93Hut_simulation) for 2D problems is implemented, in which multiple planets can be approximated with a single center of mass if the quotient between the width of a cluster of planets and the distance from the body to the center of mass is smaller than a  threshhold value <img src="/tex/20d4bcfec3faaaae8404a53c8fc76fb2.svg?invert_in_darkmode&sanitize=true" align=middle width=33.96649739999999pt height=22.831056599999986pt/>, chosen by the user. 


## Installation

