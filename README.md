# DIVAFILE Tool #
[![Build Status](https://api.travis-ci.org/s117/DIVAFILE_Tool.svg?branch=master)](https://travis-ci.org/s117/DIVAFILE_Tool)

This tool can create or extract the DIVAFILE in Project DIVA F2 and Project DIVA X.

## Usage ##
```
DIVAFILE_Tool [e/c] [src] [dst]
    e - extract DIVAFILE
    c - create DIVAFILE

execute DIVAFILE_Tool -v or --version to check the version
```

## About DIVAFILE ##
**All the information here about the DIVAFILE is from the game reversing work I have done. So it may not completely correct. However, if there exist any mistake, hope you could [tell](mailto:admin@0x10c.pw) me.**

The DIVAFILE is a kind of file encryption encapsulation designed by SEGA. For the moment, I think one DIVAFILE could only hold one file since currently no structure that records the directory structure has been seen in DIVAFILE and no instructions that handle multiple file case has been found in the decrypting routine in NPJB00435 (Project DIVA F2 – Japan, PlayStation 3).

The structure and endianness of the DIAVFILE on PS3 and PSV are identical. A DIVAFILE has a 16 bytes header, which structure is:

`'D' 'I' 'V' 'A' 'F' 'I' 'L' 'E' | LEN_PAYLOAD | | LEN_PLAINTEXT |`

The first 8 bytes `'DIVAFILE'` can be considered as file magic. Following `LEN_PAYLOAD` (4 bytes) and `LEN_PLAINTEXT` (4 bytes) are 32-bit unsigned integer stored in little-endian. They separately represent the size of encrypted data block and the size of original file.

The data beginning at offset 0x10 (following the header) is the encrypted data that length is `LEN_PAYLOAD`. By analyzing the executable image of NPJB00435, we can know that in current DIVAFILE, the encrypting algorithm is AES-128-ECB-NullPadding and the key is `66696c65206163636573732064656e79` ('file access deny', lol). `LEN_PAYLOAD - LEN_PLAINTEXT` equals the number of padding byte (0x00), and `LEN_PAYLOAD + 16` equals to the size of that certain DIVAFILE. Following screenshot shows the critical point of NPJB00435 where reveals the key. 

![IDA](http://i.imgur.com/PLSwVAY.png)

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
after build, you can find the binary at "Release/".

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

after build, you can find the binary at "build/bin/".

-----
This tool uses [kokke](https://github.com/kokke)'s [tiny-AES128-C](https://github.com/kokke/tiny-AES128-C) to handle AES operation.

Special thanks to [Tinko](https://github.com/TinkoLiu) and [CappU](mailto:cappuzhou@qq.com).

Copyleft (c) 2016 [S117](mailto:admin@0x10c.pw)@[Bitman Lab.](http://www.bitmen.org/)

Licensed under the **GPL-2.0** license.
