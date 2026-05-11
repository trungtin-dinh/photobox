# Photobox

Photobox is a desktop image library and image processing application developed in C++ with Qt Widgets and OpenCV.

The application was originally created as a Master project for the SIA-AMS program at Université Paul Sabatier, during the 2021-2022 academic year.

## Authors

- Oussama Khoufi
- Malik Berdier
- Trung-Tin Dinh

## Overview

Photobox combines two main functions in a single desktop interface:

1. Image library management: load, create, edit, sort and save JSON-based image libraries.
2. Image processing: apply classical image processing operations to selected images and visualize the results.

The project was designed as a Qt/C++ learning and application project. It includes a graphical interface, image metadata management, JSON library files, bundled image examples, and several classical image processing tools.

## Main features

### Image library management

Photobox can manage image collections described by JSON files. Each image entry can contain metadata such as title, cost, number, source, creation date, addition date and access status.

The interface supports:

- loading an existing image library;
- creating a new image library;
- displaying the content of a library in a table;
- sorting images by metadata fields;
- filtering images by cost or addition date;
- adding and removing images;
- saving the full library or a filtered sub-list;
- opening an image from the library for consultation or processing.

### Image processing tools

Photobox includes several classical image processing operations, including:

- brightness, contrast, shadows and highlights adjustment;
- temperature, hue, saturation and vividness adjustment;
- grayscale, sepia, negative and color channel filters;
- red, green, blue, yellow, cyan and magenta extraction;
- histogram display, normalization and equalization;
- mean, Gaussian, median and Kuwahara filtering;
- Gaussian noise and salt-and-pepper noise generation;
- edge detection with gradient and Laplacian operators;
- thresholding and segmentation;
- K-means classification;
- quantization and resolution reduction;
- nearest-neighbor and bilinear interpolation;
- Fourier transform visualization;
- Hough transform.

## Repository structure

```text
photobox/
├── DATA/
│   ├── Bibliotheques/       # JSON image libraries
│   ├── Images/              # Example images used by the libraries
│   └── Ressources/          # Icons and Qt resource files
├── headers/                 # C++ header files
├── sources/                 # C++ source files
├── ui/                      # Qt Designer interface file
├── QT-M2SIA-projet.pro      # qmake project file
├── LICENSE
└── README.md
```

## Original development environment

The project was originally developed with:

```text
Qt Creator 4.5.2
Qt 5.9.5
GCC 7.3.0, 64-bit
```

The current version has been updated to build with a modern Linux environment while keeping the original qmake-based Qt 5 project structure.

## Dependencies

On Ubuntu, install the required system dependencies with:

```bash
sudo apt update
sudo apt install build-essential g++ make pkg-config \
                 libgl1-mesa-dev libxcb-xinerama0 libxkbcommon-x11-0 \
                 libopencv-dev libjsoncpp-dev rapidjson-dev
```

The project uses:

- Qt 5 / Qt Widgets;
- OpenCV;
- JsonCpp;
- RapidJSON;
- qmake.

## Build from source

Open the project file in Qt Creator:

```text
QT-M2SIA-projet.pro
```

Then select a Release kit and run:

```text
Build -> Run qmake
Build -> Clean All
Build -> Rebuild All
```

The executable is named:

```text
Photobox-beta
```

A typical Qt Creator shadow-build folder is:

```text
../build-QT-M2SIA-projet-Desktop_Qt_5_9_5_GCC_64bit-Release/
```

You can also build from a terminal:

```bash
cd /path/to/photobox
mkdir -p ../build-QT-M2SIA-projet-Desktop_Qt_5_9_5_GCC_64bit-Release
cd ../build-QT-M2SIA-projet-Desktop_Qt_5_9_5_GCC_64bit-Release

~/Qt5.9.5/5.9.5/gcc_64/bin/qmake ../photobox/QT-M2SIA-projet.pro CONFIG+=release
make -j$(nproc)
```

Adapt the `qmake` path if Qt 5.9.5 is installed elsewhere.

## Run from source/build folder

From the build folder:

```bash
./Photobox-beta
```

The project contains path-handling logic so that the application can find the `DATA/` folder from the source tree when it is launched from a Qt Creator shadow-build folder.

## Release package structure

For a clean GitHub Release package, use the following structure:

```text
Photobox-v1.0.0-linux-x86_64/
├── Photobox-beta
├── DATA/
└── run.sh
```

A simple `run.sh` launcher can be used:

```bash
#!/bin/bash
cd "$(dirname "$0")"
./Photobox-beta
```

Make it executable with:

```bash
chmod +x run.sh
```

Then launch the application with:

```bash
./run.sh
```

## Data and JSON libraries

The image libraries are stored as JSON files in:

```text
DATA/Bibliotheques/
```

The corresponding image files are stored in:

```text
DATA/Images/
```

The JSON files may contain relative paths to images. In the current version, relative image paths are resolved from the JSON file location when possible, which makes the application easier to move or package.

## Notes

This project was developed as an academic Qt/C++ application. It is primarily intended for demonstration, learning and archival purposes.

The application focuses on classical image processing methods and does not use machine learning frameworks beyond simple K-means-style image classification tools implemented with OpenCV/C++ routines.

## License

This repository is distributed under the MIT License. See the `LICENSE` file for details.
