import sys
from PIL import Image

if __name__ == "__main__":
    im = Image.open(sys.argv[1])
    im.save(sys.argv[1].split(".")[0]+".ppm") 
