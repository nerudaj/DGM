# Setup

If you have installed the [dsh suite](https://github.com/nerudaj/dsh) and VS2017, you can run `bootstrap.bat` and everything will be done automatically. If you do not have dsh suite, do these steps:

 * Download [SFML-2.5.1](https://www.sfml-dev.org/files/SFML-2.5.1-windows-vc15-32-bit.zip) for your compiler to this folder and unzip it
 * Rename the unzipped folder to just `SFML`
 * Download current release of [dshlibs](https://github.com/nerudaj/dsh/releases/download/v0.6.2/dshlibs-0.6.2-vs2019-x64.zip) and unzip it to this folder. Rename the result to `dshlibs`
 * Download json.hpp header from 3.6.1 release of [nlohmann's json](https://github.com/nlohmann/json/releases/download/v3.6.1/json.hpp) and place it to folder called `json`

Optionally copy appropriate SFML dlls to `vsbuild\Release` and `vsbuild\Debug` (bootstrap does that).