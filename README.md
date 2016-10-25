# DIVAFILE Tool #
[![Build Status](https://api.travis-ci.org/s117/DIVAFILE_Tool.svg?branch=master)](https://travis-ci.org/s117/DIVAFILE_Tool)

This tool is made for creating and extracting the DIVAFILE designed by SEGA which is used in Project DIVA F2 and Project DIVA X.

## Usage ##
```
DIVAFILE_Tool [e/c] [src] [dst]
    e - extract DIVAFILE
    c - create DIVAFILE

execute DIVAFILE_Tool -v or --version to check the version
```

## Build ##
### Windows ###
To build on Windows with MSBuild, open the "Developer Command Prompt" of Visual Studio, go to the root directory of the project, and then:
 - Build:
```
> MSBuild DIVAFILE_Tool.sln /t:Build /p:Configuration=Release /p:Platform=x86
```
 - Clean:
```
> MSBuild DIVAFILE_Tool.sln /t:Clean /p:Configuration=Release /p:Platform=x86
```
 - Rebuild:
```
> MSBuild DIVAFILE_Tool.sln /t:Rebuild /p:Configuration=Release /p:Platform=x86
```
after build, you can find the binary at "Release/DIVAFILE_Tool.exe".

### Linux and OSX ###
To build on Linux or OSX with GCC or Clang, open terminal, go to the root directory of the project, and then:
 - Build:
```
$ make all
```
 - Clean:
```
$ make clean
```
 - Rebuild:
```
$ make rebuild
```

after build, you can find the binary at "build/bin/DIVAFILE_Tool".


## About DIVAFILE ##
**All the information about the DIVAFILE here are from the game reversing works I have done, so it might not absolutely correct. However, if you find anything wrong, hope you could [tell](mailto:admin@0x10c.pw) me.**

DIVAFILE is a kind of file encryption encapsulation designed by SEGA. And currently one DIVAFILE could only hold one file since currently no structure that records the directory information has been observed in DIVAFILE, and in the game binary(NPJB00435) I haven't found any code that handle multiple file situation in the DIVAFILE decryption routine. The file structure and endianness of DIAVFILE on PS3 and PSV are identical.

Each DIVAFILE has a 16 bytes file header, which structure is shown as following:

`'D' 'I' 'V' 'A' 'F' 'I' 'L' 'E' | LEN_PAYLOAD | | LEN_PLAINTEXT |`

First 8 bytes `'DIVAFILE'` can be considered as file magic. Following `LEN_PAYLOAD` (4 bytes) and `LEN_PLAINTEXT` (4 bytes) are 32-bit unsigned integer stored in little-endian. They separately record the size of encrypted data block and the size of original file.

The data beginning at offset 0x10 (following the header) is the encrypted data which length is `LEN_PAYLOAD`. By reversing the executable binary of NPJB00435 (Project DIVA F2 – Japan) on PlayStation 3, we can know in the current DIVAFILE, the AES-128-ECB-NullPadding is used as the encryption algorithm and the AES key is `66696c65206163636573732064656e79` ('file access deny', lol). `LEN_PAYLOAD - LEN_PLAINTEXT` equals the number of padding byte (0x0), and `LEN_PAYLOAD + 16` equals to the size of that certain DIVAFILE. Following image shows the disassembly point of NPJB00435 where reveals the key. 

![IDA](http://i.imgur.com/PLSwVAY.png)


-----
[kokke's](https://github.com/kokke) [tiny-AES128-C](https://github.com/kokke/tiny-AES128-C) has been used in this tool to handle the AES encryption and decryption operation.

Special thanks to [Tinko.](https://github.com/Lazy6Studio) and [CappU](mailto:cappuzhou@qq.com).

Copyright (c) 2016 [S117](mailto:admin@0x10c.pw)@[Bitman Lab.](http://www.bitmen.org/)

Licensed under the **GPL-2.0** license.
