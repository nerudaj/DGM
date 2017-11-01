# DGM
## Introduction
Is a SFML extension library that aims on faster development of 2D games. It's features include (but
are not limited to):

* Animations
* Particle Effects
* Collisions
* Design Patterns
* Any many more!

DGM is completely free, feel free to use it as you see fit (just mention the author please :) ).

## Installation

You can either use official releases or build the library for yourself. I am not experienced in terms
of Makefile generations and similar stuff (if you like the project, you can help with that) so the
only version that can be obtained is for windows, compiled with Visual Studio 2015.

If you wish to build the library for yourself, make sure you install SFML into 3rdParty directory.
Output files will be compiled into `DGM\include` and `DGM\lib`.

## Documentation

The code is documented using Doxygen, so you can generate documentation. Also I will do my best to
populate the wiki with tutorials and usage articles.

## Usage

DGM works currently with SFML-2.4.2. The setup is quite simple. Create a new project and let it know
where are folders: `SFML-2.4.2\include`, `DGM\include`, `SFML-2.4.2\lib`, `DGM\lib`. Then just write
following code at the beginning of main.cpp:
```c++
#include <DGM\dgm.hpp>

#ifdef _DEBUG
#pragma comment(lib, "dgm-d.lib")
#else
#pragma comment(lib, "dgm.lib")
#endif
```

DGM will automatically link all SFML headers (except Net) and link all SFML libraries (except Net).
Now you can code!