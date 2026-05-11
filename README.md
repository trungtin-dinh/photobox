# Photobox

Photobox is a Qt/C++ desktop application for experimenting with classical image processing operations through a graphical interface.

The project was developed as a Master project for the SIA-AMS program at Université Paul Sabatier during the 2021-2022 academic year.

## Authors

- Oussama Khoufi
- Malik Berdier
- Trung-Tin Dinh

## Overview

Photobox provides a graphical environment for loading images, applying image processing operations, visualizing results, and working with predefined image libraries stored in JSON files.

The application was originally developed as an academic project. It focuses on classical image processing methods rather than machine learning, and was designed to expose the different steps of image manipulation, filtering, transformation, segmentation, and analysis through a Qt interface.

## Main features

- Load and display images from predefined libraries.
- Apply classical image processing operations.
- Visualize input and output images.
- Work with image libraries described by JSON files.
- Use embedded resources and local data folders.
- Build and run as a native Linux Qt/C++ application.
- Distribute the application as a Linux AppImage release.

## Repository structure

```txt
photobox/
├── DATA/
│   ├── Bibliotheques/
│   ├── Images/
│   └── Ressources/
├── headers/
├── sources/
├── ui/
├── QT-M2SIA-projet.pro
├── LICENSE
└── README.md
```

The main source files are located in:

```txt
sources/
headers/
ui/
```

The application data are located in:

```txt
DATA/
```

The Qt project file is:

```txt
QT-M2SIA-projet.pro
```

## Recommended usage: AppImage release

For regular users, the recommended way to run Photobox is to download the latest AppImage from the GitHub Releases page.

After downloading the AppImage, run:

```bash
chmod +x Photobox-v1.0.0-x86_64.AppImage
./Photobox-v1.0.0-x86_64.AppImage
```

The AppImage package includes the application, the required data files, and the main runtime libraries needed to launch the program on a compatible Linux x86_64 system.

No manual Qt installation is required when using the AppImage.

## Build from source

Developers who want to compile the project from source need a Qt/C++ development environment.

This project was originally developed with:

```txt
Qt Creator 4.5.2
Qt 5.9.5
GCC 7.3.0
Linux x86_64
```

On a modern Ubuntu system, install the required development packages:

```bash
sudo apt update
sudo apt install build-essential g++ make pkg-config \
                 libopencv-dev libjsoncpp-dev rapidjson-dev \
                 libgl1-mesa-dev libxcb-xinerama0 libxkbcommon-x11-0
```

Then open the project file in Qt Creator:

```txt
QT-M2SIA-projet.pro
```

Select a Qt 5 kit, run qmake, and build the project in Release mode.

## Manual build from terminal

The project can also be built from the terminal with qmake:

```bash
cd /path/to/photobox

mkdir -p build-release
cd build-release

/path/to/Qt/5.9.5/gcc_64/bin/qmake ../QT-M2SIA-projet.pro CONFIG+=release
make -j$(nproc)
```

The executable is generated in the build directory.

## Runtime data

Photobox uses the `DATA/` folder for image libraries, resources, and example images.

When running from source, the application expects the project data folder to remain available:

```txt
DATA/
```

When using the AppImage release, the data folder is packaged with the application.

## Release package

The official Linux release is distributed as an AppImage:

```txt
Photobox-v1.0.0-x86_64.AppImage
```

This format is preferred because it avoids asking users to manually install an old Qt version or project-specific runtime dependencies.

A simple compressed release folder may also be produced for development purposes, but the AppImage is the recommended format for end users.

## Notes for developers

This project was initially written for an older Qt/OpenCV environment and has been updated to build on a more recent Ubuntu system.

Some parts of the code reflect its academic project origin, with explicit image processing functions implemented manually for clarity and experimentation.

## License

This project is distributed under the MIT License. See the `LICENSE` file for details.
