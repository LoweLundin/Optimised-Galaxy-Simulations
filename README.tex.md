# Optimised Galaxy Simulations in C

This project was part of the course High Performance Computing taken at Uppsala University during 2019. Mind that the code was produced at an earlier stage in my education and that everything might not be best practice.

The project is based on Newton's law of universal gravitation,$F=G\frac{m_1m_2}{r^2}$, with every dot on the display representing a planet with a unique position, mass and velocity.

The project is split in two parts:

Star-by-star, a "vanilla", unoptimised version, where the gravitational pull on each planet is computed for each other planet individually, by making use of Newton's third law, 
\begin{displayquote}
When one body exerts a force on a second body, the second body simultaneously exerts a force equal in magnitude and opposite in direction on the first body.
\end{displayquote}
we can reduce the number of computations needed per timestep to ,$\frac{N^2}{2}$, , where N is the number of stars, but complexity is still $\mathcal{O}(N^2)$
 
Barnes-Hut-Pthreads, where the [Barnes-Hut Approximation algorithm](https://en.wikipedia.org/wiki/Barnes%E2%80%93Hut_simulation) for 2D problems is implemented, in which multiple planets can be approximated with a single center of mass if the quotient between the width of a cluster of planets and the distance from the body to the center of mass is smaller than a  threshhold value $\theta_{max}$, chosen by the user. 


## Installation

