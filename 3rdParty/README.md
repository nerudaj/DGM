# Setup

If you have installed the [dsh suite](https://github.com/nerudaj/dsh) and VS2017, you can run `bootstrap.bat` and everything will be done automatically. If you do not have dsh suite, do these steps:

 * Download SFML-2.5.1 for your compiler to this folder and unzip it
 * Rename the unzipped folder to just `SFML`
 * Download current release of [dshlibs](https://github.com/nerudaj/dsh/releases/download/dshlibs_v0.1.1/dshlibs-0.1.1.zip) and unzip it to this folder

Optionally copy appropriate SFML dlls to `vsbuild\Release` and `vsbuild\Debug` (bootstrap does that).