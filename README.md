Description
===========

This project is a basic implementation of ["Real-time Realistic Ocean Lighting and Seamless Transitions from Geometry to BRDF"](http://www-ljk.imag.fr/Publications/Basilic/com.lmc.publi.PUBLI_Article@125fe8a8322_1ac3379/article.pdf).

Notes
=====

I started tinkering on this project many years ago (in 2012). It currently has no lighting effects
but implements a wireframe ocean wave simulation using the [Projected
Grid](https://fileadmin.cs.lth.se/graphics/theses/projects/projgrid/) method from 2004.

Some possible future improvements:
1. The vertices along the edges of the projected grid swim in-and-out of the viewport in a
   noticeable manner. Could implement something to keep the edges of the grid outside the viewport.
2. Instead of using a projected grid, the grid could be implemented in world space coordinates with
   some type of gradual LOD applied based on distance. A more modern GPU tesselation pipeline could
   also be used.
3. The ocean surface needs actual shading.
4. Qt and OpenGL are pretty old in this day and age. I will probably port this first to Vulkan or
   Metal with a better UI toolkit.
