# Cross-Engine Ver: 0.0.1
My first ever engine (and definitely not the last). Limitations and goals of software is based off of John Carmacks DOOM 1993 engine.

# Currently Implementing:
* Portal Rendering
* Floors/Ceilings
* Cleaning Up Code
* z-buffering/BSP

# Features I plan on having:
* Physics (collision detecting, explosions forces, maybe even some fancy gibbing)
* Map designer/editor, with an option to pick portal (moving walls n fancy stuff, at the cost or rendeering time) or BSP rendering (walls can only "move" vertially, but super fast rendering). Will have it's own file format
* Objects and Sprites.
* The engine will be like an API, allowing users to create their own objects using inheritance

# The Math
###### (I am writing a paper that explain the mathematics and methods of this engine in depth, but until that is finished I'll skim over it in this section)
* I primarily use Algebra and Precalculus. Matricies are good, but by deriving formulas myself when I have the specific goals of the engine in mind, good ol' high school math with a drop of calculus can allow for some great optimizations. In fact the entire idea of the game engine is based upon the fact that algebreic calculation are fast to compute, and give a lot of options as to what you can do.
  * It is possible to think of 3D as a collection of infinite 2D planes along a z-axis, allowing for two equations connecting the x and z, and y and z planes. The equations connecting them are: z = mx + b and y = mz + b.
  * This makes it easy to calculate intersections between lines and points (representing walls and entities). Clipping become a matter of finding the intersection between the wall and the fov lines. (g_ClipLineToView). It also makes interpolation easier.
