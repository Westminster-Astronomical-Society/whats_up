# What's Up (Stellarium Script)

This repo contains an example Stellarium script for a "What's Up" presentation in the planetarium. It includes a file with some common objects and a `settings.inc` with some helper functions and settings for the presentation.

These examples require Stellarium 26.2 or later for the `core.waitForKeypress()` function to work.

## Usage

Place the script in Stellarium's user scripts folder and load it from the scripts tab in the configuration window (F2) or the Scripting Console (F12).

Specify the date and time, location, and landscape at the top of the script. It will call `setup()` from `settings.inc` to initialize the presentation.

After initialization, you can use the provided functions to navigate the sky, highlight constellations, and draw pointers between celestial objects.

`drawPointer(obj1Name, obj2Name, steps = 40, colorHex = "#FF0000")` - Draws a line along a great circle arc between two named objects. Set the number of interpolation steps and the color of the line.

`goToAltAz(obj, duration = 4, showLines = false, showArt = false)` - Moves the azimuth of an object with a 180° field of view and the zenith at the center of the view, optionally showing its constellation.

`zoomToObject(obj, fov, duration)` - Zooms in on the specified object to the given field of view.

`showConstellation(Const, showLines = true, showArt = true)` - Selects and isolates the specified constellation, optionally showing its lines and art.

`objectGoTo(name, fov = 1, duration = 4, showConst = true)` - show an object, zoom in, and zoom out to initial view, optionally showing its constellation.

`constellationTour(Const, fov = 60, duration = 4, boundaries = true, objects = null)` - Tours a constellation: shows it, zooms to full constellation view, optionally shows boundaries, tours specified objects, and returns to the initial view.

Objects passed to `constellationTour` are expected to be an array of objects, each containing a `name` property for the celestial object and an `fov` property for the field of view to use when zooming to that object. For example:

```javascript
OriObjects = [ 
    {"name": "M42", "fov": 4},
    {"name": "IC434", "fov": 4}
];
```

`what's_up_2026-12-12.ssc` - Example script file for the presentation on December 12, 2026.
