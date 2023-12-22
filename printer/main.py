from PIL import Image
import random

width = 100
height = 100
# Import an image from directory: 
input_image = Image.new(mode = "RGB", size = (width, height))
pixel_map = input_image.load() 
# taking half of the width:
red = random.randint(0, 255)
green = random.randint(0, 255)
blue = random.randint(0, 255)
for i in range(width): 
    for j in range(height): 
        # getting the RGB pixel value. 
        # r, g, b, p = input_image.getpixel((i, j)) 
        # Apply formula of grayscale: 
        # grayscale = (0.299*r + 0.587*g + 0.114*b) 
        # setting the pixel value. 
        red = random.randint(0, 255)
        green = random.randint(0, 255)
        blue = random.randint(0, 255)
        pixel_map[i, j] = (int(red), int(green), int(blue)) 
# Saving the final output 
# as "grayscale.png": 
input_image.save("test.png", format="png")
# use input_image.show() to see the image on the 