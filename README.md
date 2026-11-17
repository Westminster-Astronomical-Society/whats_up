# What's Up (Stellarium Script)

This repo contains an example Stellarium script for a "What's Up" presentation in the planetarium. It includes a file with some common objects and a `settings.inc` with some helper functions and settings for the presentation.

## Usage

Place the script in Stellarium's user scripts folder and load it from the scripts tab in the configuration window (F2) or the Scripting Console (F12).

Specify the date and time, location, and landscape at the top of the script. It will call `setup()` from `settings.inc` to initialize the presentation.

After initialization, you can use the provided functions to navigate the sky, highlight constellations, and draw pointers between celestial objects.
