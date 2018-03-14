# Setting up a project

## Table of contents
1. [Dependencies](#dependencies)
2. [Limit modules](#limit-modules)

## Dependencies

After setting up your empty VS project, you have to link basic dependencies - SFML
and DGM. Both are linked the same way, so refer to [official guide](https://www.sfml-dev.org/tutorials/2.4/start-vc.php) for
more info. For now, link only include and lib folders and then create your `Main.cpp` file:

```c++
#include <DGM\dgm.hpp>

#ifdef _DEBUG
#pragma comment(lib, "dgm-d.lib")
#else
#pragma comment(lib, "dgm.lib")
#endif

int main(int argc, char *argv[]) {
	return 1;
}
```

This is all you initially have to do. Everything from SFML and DGM is linked and
working after this point.

## Limit modules

Base SFML modules as Window and System are imported always, but you can limit
imports of Audio and Network modules, simply by defining a macro `NO_AUDIO` and
`NO_NETWORK`. This will affect which parts of SFML will be available, size of the
output binary and also which dynamic libraries you need to supply with your
binary.