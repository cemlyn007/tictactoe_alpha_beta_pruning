# Dependencies
## libtinfo.so.5
Ubuntu 24 doesn't have this easily accesible, but the LLVM toolchain still depends on it otherwise Bazel will fail to build. Potential work is being done to remove this dependency which would be great for creating a more hermatic build.
For now the best course of action is to run the following:
```
wget http://archive.ubuntu.com/ubuntu/pool/universe/n/ncurses/libtinfo5_6.4-2_amd64.deb && sudo dpkg -i libtinfo5_6.4-2_amd64.deb && rm -f libtinfo5_6.4-2_amd64.deb
```
Source from the best source of the internet: `https://www.reddit.com/r/Ubuntu/comments/1cm97bg/libncurses5dev/`

# Future Plans
## Use C++ Modules
Currently Bazel doesn't provide support out of the box for C++ modules. It is probably best to wait for the future to be merged on the Bazel side but worse case could dig into how to write my own Bazel rule for it.
