# Vaporwave Generator

This is an openFrameworks application that applies Vaporwave-style effects to images. You can apply effects like color mapping, glitch, scanlines, and noise in real-time.

## Features

- **Color Mapping**: Adjusts the image's color palette to a classic Vaporwave aesthetic using a Look-Up Table (LUT).
- **Glitch Effect**: Creates horizontal glitch artifacts on the image.
- **Scanlines**: Overlays scanlines to mimic the look of a CRT screen.
- **Noise**: Adds random noise for a lo-fi texture.
- **Real-time Control**: All parameters can be tweaked in real-time via a GUI.
- **Drag & Drop**: You can load your own images by dragging and dropping them onto the application window.

## Usage

1.  Launch the application.
2.  A sample image (`pic.png`) will be loaded by default.
3.  Use the GUI on the left to toggle and adjust the different effects.
4.  Drag and drop your own image file onto the window to apply effects to it.
5.  Press the `s` key to save the current image with effects applied. The saved file will be named `saved_image.png` and will be located in the `bin/data` directory.

## Controls

-   **GUI**: Control all effect parameters.
-   **Drag & Drop**: Load a new image.
-   **'s' key**: Save the current canvas as `saved_image.png`.

## Parameters

You can control the following parameters through the GUI:

-   **Color Mapping**
    -   `Deep Blue`, `Magenta`, `Cyan`, `White`: Define the key colors for the gradient map.
    -   `Thresh 1`, `Thresh 2`: Adjust the transition points between colors.
    -   `Apply Effect`: Toggle the color mapping effect.
-   **Glitch Settings**
    -   `Glitch Count`: Number of glitch lines.
    -   `Max Offset`: Maximum horizontal displacement for glitches.
    -   `Min Height`, `Max Height`: The range for the height of each glitch line.
    -   `Apply Glitch`: Toggle the glitch effect.
-   **Texture Settings**
    -   `Scanline Density`: The spacing of the scanlines.
    -   `Scanline Alpha`: The opacity of the scanlines.
    -   `Apply Scanlines`: Toggle the scanline effect.
    -   `Noise Count`: The amount of noise particles.
    -   `Noise Alpha`: The opacity of the noise.
    -   `Apply Noise`: Toggle the noise effect.

## How to Build

This project is developed with openFrameworks. Please refer to the official openFrameworks documentation for instructions on how to set up your environment and build projects.

1.  Download and set up openFrameworks (v0.12.0 or later recommended).
2.  Place this project directory inside `of_v0.12.0_vs_64/apps/myApps/`.
3.  Open the solution file (`.sln`) in Visual Studio and compile.

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.
