# qt-opengl

![main window screenshot](https://raw.githubusercontent.com/Maktel/qt-opengl/master/promo.png)

## List of features:
* loading a file
* saving to a file
* scrollable display area
* simple point drawing
* ability to zoom in canvas
* drawing grid (directly in the picture)
* debugging window for certain operations like filling

### Drawing:
* line (Bresenham's, Xiaolin's Wu antialiased)
* circle (Bresenham's, approximating with lines and variable number of steps)
* polygon
* Bezier curve
* basis spline (unlimited points)

### Painting:
* filling with colour (recursive, stack-based flood fills, scanline)
* vertical and horizontal 2 colour gradients (variable number of steps)

### Transformations:
* translation
* rotation (relative to (0, 0) or centre of the image)
* scaling (relative to (0, 0) or centre of the image)
* shearing (relative to (0, 0) or centre of the image)
* interpolation (nearest neighbour, bilinear)

### TODO features:
* drawing grid in another widget instead of directly in the image
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
