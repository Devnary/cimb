# cimb
Converting image to minecraft banner

## Description

Program quite slow depending on the image resolution and complexity.

The input image has to be PPM (P6) format. Width has to be multiple of 20, and height multiple of 40.

`minecraft_banners.ptrn` contains all minecraft banner patterns including the banner base texture.

## Usage (linux)
```sh
gcc converter.c -o converter
./converter minecraft_banners.ptrn [Your image PPM]
```
