#include <stdio.h>
#include <stdlib.h>

u_int8_t palette[48] = {0xbd,0xc1,0xc1, 0xbd,0x61,0x15, 0x97,0x3a,0x8f, 0x2b,0x88,0xa5, 0xc0,0xa3,0x2d, 0x61,0x96,0x15, 0xb7,0x69,0x80, 0x35,0x3c,0x3e, 0x76,0x76,0x72, 15,0x76,0x76, 0x68,0x25,0x8b, 0x2c,0x32,0x81, 0x63,0x3f,0x25, 0x47,0x5e,0, 0x85,0x22,0x1c, 0x15,0x15,0x17};

u_int8_t  ptrnI[3];
u_int8_t* ptrnV;

u_int16_t ppmI[3];
u_int8_t* ppmV;

u_int16_t istrI[2];
u_int8_t* istrV;

void loadPTRN(u_int8_t* path) {
    
    u_int8_t ptrnS[4];
    
    FILE* f = fopen(path, "rb");
    if (!f) {
        
        printf("%s does not exist.\n", path);
        exit(1);
    }
    
    fread(ptrnS, 4, 1, f);
    if (ptrnS[0] != 'p' || ptrnS[1] != 't' || ptrnS[2] != 'r' || ptrnS[3] != 'n') {
        
        printf("%s is not a ptrn file.\n", path);
        exit(1);
    }
    
    fread(ptrnI, 3, 1, f);
    
    printf("ptrn %d %dx%d\n", ptrnI[0],ptrnI[1],ptrnI[2]);
    
    u_int32_t size = ptrnI[0] * ptrnI[1] * ptrnI[2] * 4;
    ptrnV = malloc(size);
    fread(ptrnV, size, 1, f);
    
    fclose(f);
}

void loadPPM(u_int8_t* path) {
    
    u_int8_t ppmS[2];
    
    FILE* f = fopen(path, "rb");
    if (!f) {
        
        printf("%s does not exist.\n", path);
        exit(1);
    }
    
    fread(ppmS, 2, 1, f);
    if (ppmS[0] != 'P' || ppmS[1] != '6') {
        
        printf("%s is not a ppm (P6) file.\n", path);
        exit(1);
    }
    
    for (int8_t i = -1, j = 0, c; i < 3;) { // Read PPM header
        
        c = fgetc(f);
        if (c > 32) {
            
            ppmI[i] += (c - 48) * (u_int16_t[4]){1000, 100, 10, 1}[j];
            j++;
            continue;
        }
        
        for (u_int8_t k = 0; k < 3 && j < 4; ppmI[i] *= .1,j++,k++);
        
        i++;
        j = 0;
    }
    
    istrI[0] = ppmI[0],
    istrI[1] = ppmI[1];
    istrV = malloc(istrI[0] / 20 * istrI[1] / 40 * 15); // 15 = 6 (12) patterns+colors + 1 base color + pos
    
    printf("ppm %d %dx%d\n", ppmI[2],ppmI[0],ppmI[1]);
    
    u_int32_t size3 = ppmI[0] * ppmI[1] * 3;
    ppmV = malloc(ppmI[0] * ppmI[1] * 4);
    
    for (u_int32_t i = 0; i < size3; i += 3, ppmV[i + i / 3 + 3] = 255) // RGB to RGBA
        fread(ppmV + i + i / 3, 3, 1, f);
}

void exportPPM(u_int8_t* buffer, u_int32_t w, u_int32_t h) {
    
    FILE* f = fopen("out.ppm", "wb");
    fprintf(f, "P6\n%d %d\n255\n", w, h);
    fclose(f);
    
    u_int32_t size  = w * h,
              size3 = w * h * 3;
    u_int8_t buf[size3];
    for (u_int32_t i = 0; i < size3; buf[i] = buffer[i + i / 3],i++); // RGBA to RGB
    
    
    f = fopen("out.ppm", "a");
    
    for (u_int32_t i = 0; i < 3; i++)
        fwrite(buf + i * size, size, 1, f);
    
    fclose(f);
}

void clU8Arr(u_int8_t* buffer, u_int32_t size) { // clear unsigned char array (fill 0)
    
    for (u_int32_t i = 0; i < size; buffer[i] = 0, i++);
}

void cpU8Arr(u_int8_t* output, u_int8_t* input, u_int32_t size) { // copy unsigned char array
    
    for (u_int32_t i = 0; i < size; output[i] = input[i], i++);
}

void getPPMChunk(u_int8_t* buffer, u_int32_t x, u_int32_t y, u_int32_t w, u_int32_t h) {
    
    for (u_int32_t i = 0; i < h; i++)
        for (u_int32_t j = 0; j < w; j++)
            buffer[(j + i * w) * 4    ] = ppmV[(j + x + (i + y) * ppmI[0]) * 4    ],
            buffer[(j + i * w) * 4 + 1] = ppmV[(j + x + (i + y) * ppmI[0]) * 4 + 1],
            buffer[(j + i * w) * 4 + 2] = ppmV[(j + x + (i + y) * ppmI[0]) * 4 + 2],
            buffer[(j + i * w) * 4 + 3] = ppmV[(j + x + (i + y) * ppmI[0]) * 4 + 3];
}

void setPPMChunk(u_int8_t* buffer, u_int32_t x, u_int32_t y, u_int32_t w, u_int32_t h) {
    
    for (u_int32_t i = 0; i < h; i++)
        for (u_int32_t j = 0; j < w; j++)
            ppmV[(j + x + (i + y) * ppmI[0]) * 4    ] = buffer[(j + i * w) * 4    ],
            ppmV[(j + x + (i + y) * ppmI[0]) * 4 + 1] = buffer[(j + i * w) * 4 + 1],
            ppmV[(j + x + (i + y) * ppmI[0]) * 4 + 2] = buffer[(j + i * w) * 4 + 2],
            ppmV[(j + x + (i + y) * ppmI[0]) * 4 + 3] = buffer[(j + i * w) * 4 + 3];
}

void convert() {
    
    u_int32_t paletteSize = sizeof(palette),
              patternSize = ptrnI[1] * ptrnI[2] * 4,
              patternsMax = 6,
              patternDone, error, bestError, offset;
    u_int8_t chunk[patternSize], currentPattern[patternSize], lastPatterns[patternsMax + 1][patternSize];
    
    for (u_int32_t y = 0; y < ppmI[1]; y += 40) {
        for (u_int32_t x = 0; x < ppmI[0]; x += 20) {
            
            bestError = 4294967295,
            patternDone = 0;
            
            getPPMChunk(chunk, x, y, ptrnI[1], ptrnI[2]);
            
            for (u_int32_t i = 0; i < paletteSize; i += 3) { // base color
                
                error = 0;
                clU8Arr(currentPattern, patternSize);
            
                for (u_int32_t j = 0; j < patternSize; j += 4) {
                    
                    float R = (float)ptrnV[j    ] * 0.00392156862745098 * palette[i    ],
                          G = (float)ptrnV[j + 1] * 0.00392156862745098 * palette[i + 1],
                          B = (float)ptrnV[j + 2] * 0.00392156862745098 * palette[i + 2],
                          A = (float)ptrnV[j + 3] * 0.00392156862745098;
                    
                    currentPattern[j    ] = R * A + (float)currentPattern[j    ] * (1 - A),
                    currentPattern[j + 1] = G * A + (float)currentPattern[j + 1] * (1 - A),
                    currentPattern[j + 2] = B * A + (float)currentPattern[j + 2] * (1 - A);
                    
                    error += abs(currentPattern[j] - chunk[j]) + abs(currentPattern[j + 1] - chunk[j + 1]) + abs(currentPattern[j + 2] - chunk[j + 2]);
                }
                
                if (error < bestError) {
                    
                    bestError = error;
                    cpU8Arr(lastPatterns[0], currentPattern, patternSize);
                }
            }
            
            for (u_int32_t n = 1; n < patternsMax + 1; n++) { // adding patterns
                
                bestError = 4294967295;
                
                for (u_int32_t l = 1; l < ptrnI[0]; l++)
                    for (u_int32_t i = 0; i < paletteSize; i += 3) {
                        
                        error = 0;
                        cpU8Arr(currentPattern, lastPatterns[n - 1], patternSize);
                        
                        for (u_int32_t j = 0; j < patternSize; j += 4) {
                            
                            offset = l * patternSize;
                            
                            float R = (float)ptrnV[j + offset    ] * 0.00392156862745098 * palette[i    ],
                                  G = (float)ptrnV[j + offset + 1] * 0.00392156862745098 * palette[i + 1],
                                  B = (float)ptrnV[j + offset + 2] * 0.00392156862745098 * palette[i + 2],
                                  A = (float)ptrnV[j + offset + 3] * 0.00392156862745098;
                            
                            currentPattern[j    ] = R * A + (float)currentPattern[j    ] * (1 - A),
                            currentPattern[j + 1] = G * A + (float)currentPattern[j + 1] * (1 - A),
                            currentPattern[j + 2] = B * A + (float)currentPattern[j + 2] * (1 - A);
                            
                            error += abs(currentPattern[j] - chunk[j]) + abs(currentPattern[j + 1] - chunk[j + 1]) + abs(currentPattern[j + 2] - chunk[j + 2]);
                        }
                        
                        if (error < bestError) {
                            
                            bestError = error;
                            cpU8Arr(lastPatterns[n], currentPattern, patternSize);
                        }
                    }
                
                patternDone++;
            }
            
            setPPMChunk(lastPatterns[patternDone], x, y, ptrnI[1], ptrnI[2]);
        }
        
        printf("%d/%d\n", y + ptrnI[0]-1, ppmI[1]);
    }
    
    exportPPM(ppmV, ppmI[0], ppmI[1]);
}

int main(int argc, char** argv) {
    
    if (argc < 3) {
        
        printf("converter [ptrn] [ppm]\n");
        return 1;
    }
    
    loadPTRN(argv[1]);
    loadPPM(argv[2]);
//     resizePPM(); //TODO
    convert();
    
    free(ptrnV);
    free(ppmV);
    free(istrV);
    
    return 0;
}
