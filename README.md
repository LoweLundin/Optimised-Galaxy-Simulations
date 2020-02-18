# Optimised Galaxy Simulations in C

This project was part of the course High Performance Computing taken at Uppsala University during 2019. Mind that the code was produced at an earlier stage in my education and that everything might not be best practice.

The project is based on Newton's law of universal gravitation,<img src="/tex/ebe1a6a00f5417138a18fd859a6bb504.svg?invert_in_darkmode&sanitize=true" align=middle width=85.82971649999999pt height=23.388043799999995pt/>, with every dot on the display representing a planet with a unique position, mass and velocity.

The project is split in two parts:

1. Star-by-star, a "vanilla" unoptimised version, where the gravitational pull on each planet is computed for each other planet individually, by making use Newton's third law, 
<p align="center"><img src="/tex/64d10742081cfcb218c9a453eb28695e.svg?invert_in_darkmode&sanitize=true" align=middle width=700.2746553pt height=34.3379058pt/></p>
we can reduce the number of computations needed per timestep to <img src="/tex/f97c1a43186c0257a4914ddce10d9cda.svg?invert_in_darkmode&sanitize=true" align=middle width=18.061748099999996pt height=33.45973289999998pt/>, where N is the number of starts, but complexity is still <img src="/tex/8e90dbe2d3ca28b3ad0012cb03e7ead6.svg?invert_in_darkmode&sanitize=true" align=middle width=48.70330244999999pt height=26.76175259999998pt/>
 
2. Barnes-Hut-Pthreads, where  


## Installation

