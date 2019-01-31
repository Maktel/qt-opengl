# qt-opengl

![main window screenshot](https://raw.githubusercontent.com/Maktel/qt-opengl/master/promo.png)

## Note on usage:
All actions can be executed using top bar menu or by keyboard shortcuts. Operations like drawing or filling require using mouse to mark points. In addition to this, functions with unlimited number of points like drawing a polygon or basis spline require user to **press Space** or use context menu (invoked with RMB) to apply points selection. Gradient painting and image transformations are applied **immediately** after being triggered.

Application also **provides examples with preconfigured sets of actions**. User can enable them by focusing the canvas with a mouse click and pressing **number keys (1 - 4)**. Some of the examples create an additional debugging window to visualise internals of drawing functions and algorithms they use.

## List of features:
* loading a file
* saving to a file
* scrollable display area
* simple point drawing
* ability to zoom in canvas
* drawing a grid (directly in the picture)
* debugging window for certain operations like filling

### Drawing:
* line (Bresenham's, Xiaolin's Wu antialiased)
* circle (Bresenham's, approximating with lines and a variable number of steps)
* polygon
* Bezier curve
* basis spline (unlimited points)

### Painting:
* filling with colour (recursive, stack-based flood fills, scanline)
* vertical and horizontal 2 colour gradients (variable number of steps)

### Transformations:
* translation
* rotation (relative to (0, 0) or the centre of the image)
* scaling (relative to (0, 0) or the centre of the image)
* shearing (relative to (0, 0) or the centre of the image)
* interpolation (nearest neighbour, bilinear)

## TODO features:
* drawing grid in an another widget instead of directly in the image
* improved transformations interface (ability to set up transformations with command line like utility)
* more antialiasing functions
* ability to resize canvas using settings

### code related TODOs:
* drawing functions and pixel manipulation optimisation
* improved file manipulation (like checking for rwx permissions)
* simplification and unification of UI helper classes
* simplification of Drawer class public interface
* better UI for Android 
* removal of switch statement based application of settings
* zoom scaled image caching 
