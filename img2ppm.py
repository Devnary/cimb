import sys
from PIL import Image

if __name__ == "__main__":
    im = Image.open(sys.argv[1])
    width, height = im.size
    im = im.resize((round(width/20)*20,round(height/40)*40))
    im.save(sys.argv[1].split(".")[0]+".ppm") 
