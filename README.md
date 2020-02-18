# Optimised Galaxy Simulations in C

This project was part of the course High Performance Computing taken at Uppsala University during 2019. Mind that the code was produced at an earlier stage in my education and that everything might not be best practice.

The project is based on Newton's law of universal gravitation,<img src="/tex/ebe1a6a00f5417138a18fd859a6bb504.svg?invert_in_darkmode&sanitize=true" align=middle width=85.82971649999999pt height=23.388043799999995pt/>, with every dot on the display representing a planet with a unique position, mass and velocity.

The project is split in two parts, one "vanilla" implementation called Star-by-star, where the gravitational pull on each planet is computed for each other planet individually, meaning the complexity is 


and Newton's third law with every star pulling each

 

##Installation
