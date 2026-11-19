// Name: settings.inc
// License: CC0 1.0 Universal Public Domain
// Author: Christopher Bennett
// Version: 1.0, 2026-09-09
// Description: This file contains settings and utility functions for Stellarium scripts

// Define some colors
White = core.vec3f(1.0, 1.0, 1.0)                     // #FFFFFF
Gray20 = core.vec3f(0.8, 0.8, 0.8)                    // #CCCCCC
Gray40 = core.vec3f(0.6, 0.6, 0.6)                    // #999999  
Gray80 = core.vec3f(0.2, 0.2, 0.2)                    // #333333
Black = core.vec3f(0.0, 0.0, 0.0)                     // #000000

BrightRed = core.vec3f(0.8314, 0.1647, 0.1647)        // #D42A2A
BrightGreen = core.vec3f(0.00000, 0.56863, 0.00000)   // #009100
BrightCobalt = core.vec3f(0.00000, 0.28627, 0.56863)  // #004991
BrightPurple = core.vec3f(0.56863, 0.00000, 0.56863)  // #910091

DarkRed = core.vec3f(0.37647, 0.12549, 0.12941)       // #602021
DarkGreen = core.vec3f(0.17647, 0.28235, 0.17647)     // #2D482D
DarkSlate = core.vec3f(0.17647, 0.23137, 0.28235)     // #2D3B48
DarkPurple = core.vec3f(0.28235, 0.17647, 0.28235)    // #482D48

// Setup environment for the presentation
function setup() {
    core.debug("Setting up environment");

    // Az mount, atmosphere, landscape, no lines, labels or markers
    core.clear("natural");

    // Look West, fov 180°, zenith at the center of the view
    core.moveToAltAzi(90, 270, 3);
    StelMovementMgr.zoomTo(180, 3);

    // Clear all markers, labels, and images
    MarkerMgr.deleteAllMarkers();
    LabelMgr.deleteAllLabels();
    ScreenImageMgr.deleteAllImages();

    // Remove all sky images except the first two (default layers)
    var keys = StelSkyLayerMgr.getAllKeys();
    if (keys.length > 2) {
        for (var i = 2; i < keys.length; i++) {
            core.removeSkyImage(keys[i]);
        }
    }

    // show sky layer background images
    StelSkyLayerMgr.setFlagShow(true);

    // Set the observer location, landscape, and time
    core.setObserverLocation(loc.lon, loc.lat, loc.elev, loc.name, loc.planet);
    LandscapeMgr.setCurrentLandscapeName(landscape);
    core.setSkyCulture(skyculture);
    core.setTimeRate(0); 
    core.setDate(dtg, spec = "local");

    core.setGuiVisible(false);
}

/**
 * Toggles the visibility of the azimuthal grid and dims the 
 * colors after a three second delay.
 */
function showAzGrid(delay = 2) {
    GridLinesMgr.setFlagAzimuthalGrid(false);

    GridLinesMgr.setColorAzimuthalGrid(BrightGreen);
    GridLinesMgr.setFlagAzimuthalGrid(true);
    core.wait(delay);

    GridLinesMgr.setFlagAzimuthalGrid(false);
    core.wait(0.5);

    GridLinesMgr.setColorAzimuthalGrid(DarkGreen);
    GridLinesMgr.setFlagAzimuthalGrid(true);
}

/**
 * Toggles the visibility of the equatorial grid and dims the 
 * colors after a three second delay.
 */
function showEqGrid(delay = 2) {
    GridLinesMgr.setFlagEquatorGrid(false);

    GridLinesMgr.setColorEquatorGrid(BrightPurple);
    GridLinesMgr.setFlagEquatorGrid(true);
    core.wait(delay);

    GridLinesMgr.setFlagEquatorGrid(false);
    core.wait(0.5);

    GridLinesMgr.setColorEquatorGrid(DarkPurple);
    GridLinesMgr.setFlagEquatorGrid(true);
}

/**
 * Fast forward at specified rate, until a specified time,
 * or until "L"key is pressed.
 */
function fastForward(rate, wait = "L", spec = "local") {
    StelMovementMgr.deselection();
    var timerate = core.getTimeRate();
    core.setTimeRate(rate);
    if (wait == "L") {
        L = LabelMgr.labelScreen("Rate = " + rate + (" press 'L' to resume"), 20, core.getScreenHeight() - 60,
            visible = true, fontSize = 18, fontColor = "#666666");
        i = 0;
        do {
            core.wait(1);
            i++;
        }
        while (core.getTimeRate() <= rate);
        core.setTimeRate(timerate);
        LabelMgr.deleteLabel(L);
    }
    else {
        core.waitFor(wait, spec = spec);
        core.setTimeRate(timerate);
    }
}

// Returns a date string from the Rise/Transit/Set of an object and current date
function dateFromRTS(timeString) {
    const date = core.getDate("local").substring(0, 10);
    const time = timeString.replace("h", ":").replace("m", ":00");
    return date + "T" + time;
}

// Get Rise/Transit/Set times as date strings
function getRTS(object, angleDep = 0, eventType = "rise") {
    const rts = core.getRTS(object, angleDep, eventType);
    return dateFromRTS(rts[eventType]);
}

// Set the time based on Rise/Transit/Set
function setRTS(object, angleDep = 0, eventType = "rise") {
    const date = getRTS(object, angleDep, eventType);
    core.setDate(date, "local");
}

// Sunrise/Sunset helpers
function getSunrise() { return getRTS("Sun", 0, "rise"); }
function setSunrise() { setRTS("Sun", 0, "rise"); }
function getSunset() { return getRTS("Sun", 0, "set"); }
function setSunset() { setRTS("Sun", 0, "set"); }

// Astronomical twilight helpers
function getAstTwilightStart() { return getRTS("Sun", -18, "rise"); }
function setAstTwilightStart() { setRTS("Sun", -18, "rise"); }
function getAstTwilightEnd() { return getRTS("Sun", -18, "set"); }
function setAstTwilightEnd() { setRTS("Sun", -18, "set"); }

/**
 * Loads a sky image layer using a parameter dictionary.
 * Wraps `core.loadSkyImage` and forwards fields from `params`.
 */
function loadImage(params) {
    core.loadSkyImage(
        params.id,
        params.path,
        params.ra,
        params.dec,
        params.angSize,
        params.rotation,
        params.minRes,
        params.maxBright,
        params.visible
    );
}


// Draws pointer line along a great circle between two named objects
function drawPointer(obj1Name, obj2Name, steps, colorHex) {
    const deg2rad = Math.PI / 180.0;
    const rad2deg = 180.0 / Math.PI;
    
    // Get object information (contains RA/Dec in degrees)
    const obj1 = core.getObjectInfo(obj1Name);
    const obj2 = core.getObjectInfo(obj2Name);
    
    // Helper: RA/Dec degrees to 3D unit vector
    const toVector = (raDeg, decDeg) => {
        const ra = raDeg * deg2rad;
        const dec = decDeg * deg2rad;
        return {
            x: Math.cos(dec) * Math.cos(ra),
            y: Math.cos(dec) * Math.sin(ra),
            z: Math.sin(dec)
        };
    };
    
    const v1 = toVector(obj1["ra"], obj1["dec"]);
    const v2 = toVector(obj2["ra"], obj2["dec"]);
    
    // Use dot product to calculate angular distance
    const dot = Math.max(-1, Math.min(1, v1.x * v2.x + v1.y * v2.y + v1.z * v2.z));
    const omega = Math.acos(dot);
    const sinOmega = Math.sin(omega);
    
    // Interpolate 3D position along the great circle (Slerp)
    // (https://en.wikipedia.org/wiki/Spherical_linear_interpolation)
    for (let i = 0; i <= steps; i++) {
        const t = i / steps;
        const w1 = sinOmega < 1e-6 ? (1 - t) : Math.sin((1 - t) * omega) / sinOmega;
        const w2 = sinOmega < 1e-6 ? t : Math.sin(t * omega) / sinOmega;
        
        const x = w1 * v1.x + w2 * v2.x;
        const y = w1 * v1.y + w2 * v2.y;
        const z = w1 * v1.z + w2 * v2.z;
        
        // Convert back to RA/Dec
        const dec = Math.asin(Math.max(-1, Math.min(1, z))) * rad2deg;
        let ra = Math.atan2(y, x) * rad2deg;
        if (ra < 0) ra += 360;
        
        MarkerMgr.markerEquatorial(ra + "d", dec + "d", false, true, "disk", colorHex, 3.0);
        core.wait(0.05);
    }
}

// select constellation by name and display isolated (lines and art)
function showConstellation(Const) {

    ConstellationMgr.deselectConstellations();
    ConstellationMgr.setFlagIsolateSelected(true);
    ConstellationMgr.setFlagConstellationPick(true);

    core.selectConstellationByName(Const);
    ConstellationMgr.setFlagLines(true);
    ConstellationMgr.setFlagArt(true);
}

// move to a selected object, zoom in and back out
function goToAltAz(obj, fov=1, duration=4) {
    const shift = fov/6;
    const az = obj["azimuth"];
    
    // Move to object, initially zoomed out
    core.moveToAltAzi(90, az, duration);
    StelMovementMgr.zoomTo(180, duration);
    core.wait(duration);
    core.waitForKeypress();

    // Zoom in (shift down for dome)
    core.moveToObject(obj["name"],duration, shift);
    StelMovementMgr.zoomTo(fov, duration);
    core.wait(duration);
    ConstellationMgr.setFlagLines(false);
    ConstellationMgr.setFlagArt(false);
    core.waitForKeypress();

    // Zoom out
    core.moveToAltAzi(90, az, duration);
    StelMovementMgr.zoomTo(180, duration);
    core.wait(duration);
}

// wrapper function to go to an object, optionally showing its constellation
function objectGoTo(name, fov=1, duration=4, Const=true) {
    var obj = core.getObjectInfo(name);

    if (Const) {
        showConstellation(obj["iauConstellation"]);
    } else {
        ConstellationMgr.deselectConstellations();
        ConstellationMgr.setFlagLines(false);
        ConstellationMgr.setFlagArt(false);
    }

    goToAltAz(obj, fov, duration);
}
