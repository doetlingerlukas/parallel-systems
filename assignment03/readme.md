# Assignment 3, due October 30th 2019

The goal of this assignment is to extend the heat stencil application and measure its performance.

## Exercise 1

This exercise consists in extending the heat stencil application of Assignment 2 to two and three dimensions.

### Tasks

- Extend the heat stencil application to the two- and three-dimensional cases and name them `heat_stencil_2D` and `heat_stencil_3D`.
- Provide sequential and MPI implementations and run them with multiple problem and machine sizes.
- How can you verify the correctness of your applications?


### Sequential versions of 2D and 3D heat stencils

The update function of the 2D version was influenced by:

* https://www5.in.tum.de/forschung/simlab/course2010_files/course_mat/2010-10-04-gatzhammer-SimLabIterative.pdf slide 14, and
* https://github.com/allscale/allscale_api/wiki/HeatStencil


## Exercise 2

This exercise consists in measuring all heat stencil variants (1D, 2D and 3D) to get a grasp of their performance behavior.

### Tasks

- Measure the speedup and efficiency of all three stencil codes for varying problem and machine sizes/mappings. Consider using strong scalability, weak scalability, or both. Justify your choice.
- Illustrate the data in appropriate figures and discuss them. What can you observe?
- Bonus question: Measure and illustrate an application throughput metric. What can you observe?

## General Notes

All the material required by the tasks above (e.g. code, figures, etc...) must be part of the solution that is handed in. Your experiments should be reproducible and comparable to your own measurements using the solution materials that you hand in. For source code, please provide a makefile or other, intuitive means of compiling with the required flags and settings.

**Every** member of your group must be able to explain the given problem, your solution, and possible findings. You may also need to answer detailed questions about any of these aspects.

**Please run any benchmarks or heavy CPU loads only on the compute nodes, not on the login node.**
If you want to do some interactive experimentation, use an *interactive job* as outlined in the tutorial. Make sure to stop any interactive jobs once you are done.
