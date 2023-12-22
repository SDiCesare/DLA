from PIL import Image
import csv
import glob
import os

def make_gif(frame_folder):
    frames = [Image.open(image) for image in glob.glob(f"{frame_folder}/*.png")]
    frame_one = frames[0]
    frame_one.save("test.gif", format="GIF", append_images=frames,
               save_all=True, duration=250, loop=0)

def make_frames(width, height, steps_folder):
    step = 0
    for filename in os.listdir(steps_folder):
        with open(os.path.join(steps_folder, filename), 'r') as data_file:
            image = Image.new(mode = "RGB", size = (width, height))
            pixel_map = image.load() 
            csv_reader = csv.reader(data_file, delimiter=',')
            data = list(csv_reader)
            for i in range(width):
                for j in range(height):
                    if int(data[i][j]) >= 0:
                        pixel_map[i, j] = (255, 0, 0)
                    else:
                        pixel_map[i, j] = (0, 0, 0)
            image.save("frames/" + str(step) + ".png", format="png")
            step = step + 1

if __name__ == '__main__':
    if not os.path.exists("steps"):
        os.makedirs("steps")
    if not os.path.exists("frames"):
        os.makedirs("frames")
    make_frames(100, 100, "steps")
    make_gif("frames")