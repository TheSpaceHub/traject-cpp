# traject-cpp

This project allows for large-scale trajectory calculations. It uses the RK4 method to solve the differential equations needed to compute the trajectories and displays them in-console using an orthographic projection (no weird conic math for perspective). The program provides 2 modalities.

### Trajectory simulation

Given the origin and the velocity of the projectile, calculates where it will land and how long it will take. Precision is mostly limited by compute (step size). Does not take into account altitudes, as the Earth is round enough and the program would need a lot of geospatial data to approximate the altitude of the landing zone.

### Trajectory calculation

Given the origin and the destination, calculates the velocity needed for the projectile to complete the trajectory. This is done using gradient descent (which works suprisingly well in this case). Speed is minimized by taking advantage of the probable convexity of the function involved in calculations by constructing a parabola through the smallest points and using its minimum.


Both of these implement the same computational physics engine, which has been optimized for speed and memory efficiency with the constraint of not decreasing precision. The program also includes a small visualization framework that uses console characters as pixels to display the trajectory. It is recommended to zoom out in order to achieve a better visual effect.

## Usage

It is enough to download the files and compile them. There are no external libraries needed.
