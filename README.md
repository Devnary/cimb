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

## Techinical
Technical information and explanations

#### PTRN file format structure
```
4b : signature
3b : infotmations (count, width, height)
...: datas
```

#### Convertion
The image is divided into blocks of 20x40. Then compared to each pattern separately with each colors and the previous pattern, 6 times.
