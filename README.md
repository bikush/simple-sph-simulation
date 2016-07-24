# Simple SPH Simulation

This is a C++/OpenGL implementation of an SPH fluid system. The simulation itself is run on CPU and can handle 1k-2k particles in real-time (this is of course dependant on the machine running the simulation). The fluid can be visualised as a point cloud or with marching cubes.

## SPH

The actual fluid simulation is a direct implementation of the SPH with minor tweaks to avoid unnecessary calculations such as square roots or multiplying everything with the same value. There are couple of variants in this repo, a 2D version, a 3D "clean" version and a 3D with a bunch of optimizations. 2D and the clean 3D implementations are not removed from the repo because they are easier to examine.

Implementation of the SPH that is used to simulate the fluid has a grid with cells the size of the smoothing length. This helps limit the number of checks for each particle. Kernels are inlined and cannot be changed dynamically. To prevent particles from escaping there are surfaces that can be added to the system, currently there is only an axis aligned box and a plane. Calculations require a lot of multiplication with particle mass and division by particle densities, that has been simplified to a multiplication with the particle volume that is precalculated before every step.

## Issues

There are some miscalculations and invalid forces may occur on some particles. Surfaces/objects within the fluid are a bit clumsy and limited. Particles tend to stick to the bottom and are not prone to "jumping" off, this means that when a surge of particles encounters a wall, they will not splash up instead they will just stop. The whole simulation is quite limited with the number of particles and is thus a bit boring, making it parallel would be nice. There is no way for the user to interact with the fluid besides adding particles.

## Drawing

The fluid can be drawn as a Point Cloud (PC):
![Point cloud fluid](http://i.imgur.com/i9f044P.png)

Or as a dynamic Marching Cube (MC) surface:
![SPH splash](http://i.imgur.com/wPoU3Y4.png)

PC draws a single point for every particle in the fluid and a geometry shader spawns quads with a simple transparent image.

MC shader gets a grid of values (0,2) and for every cell in that grid outputs the appropriate set of triangles. There is a threshold value that determines if a point in a grid is used to generate triangles in the adjacent cells. Triangle points are moved/adjusted dependent on the values in the grid to achieve a more rounded/smoothed look.

For 1k particles in a stable, calm state the average per frame drawing time for PC is 0.12 milliseconds and for MC 2.86 milliseconds.

Video (youtube, click on image):

[![SPH simulation video](https://img.youtube.com/vi/7GkNf6Wjx9I/0.jpg)](https://www.youtube.com/watch?v=7GkNf6Wjx9I)

## Running

Currently this is a Windows/VS2015 only project.

## Conclusion

This project was started in 2012 for my Master Thesis on Faculty of Electrical Engineering and Computing in Zagreb. Perhaps I should have rewritten the whole thing, but I wanted to see my old mistakes and learn from them. My motivation in putting it on GitHub was to fix the original and have a decent entry for my portfolio. This project is not intended to be extra fast or smart or flashy.


