# Bmanim

The file structure is in android phone, where the access to the main storage is in ~/storage/shared.

The program must be executed in a folder containing bmp_files folder. Then it will generate a video in ~/storage/shared with the specified characteristics.

## To create your own animations

The arguments to the function call to langton(); is in main.c, it takes 7 arguments:

width of the grill, height of the grill, scale of the image (this is explained below), limit of steps, fps of the video, how many steps each frame jumps, and the maximum amout of frames that can be generated in streak.

The third argument (res) is a scaller, what the program does is to multiply the width and the height of the grill by the scale, just to make each pixel of the animations be res * res wide, this if because if you want to make a full hd video, then you will have to put a 1920x1080 grill, and the animation will look too small; so for a full hd quality output you may not want to put a grill of 1920x1080. but instead a 128x72 grill and res = 15, wich makes an output of a full hd 128x72 grill.

The fourth argument is simple, if you put as limit 1000, the animation will be limited at 1000 steps.

The fifth argument if also simple: how many frames per second the final video will be at.

The sixth was explained above.

To explain the seventh argument I have to explain how the program works: The program generates each frame jumping sixth arguments steps per frame stoping when the animation finishes or the seventh argument has been exceeded, then the program makes a call to ffmpeg to combine all those frames into a .mp4 video format, then the frames that has been generated are deleted and the program proceeds to simulate the ant again, generating frames and frames until seventh argument or fourth argument are exceeded (again), and generates another video and deletes again all the splited frames, on and on until finally fourth argument is exceeded, then it generates the final video and another ffmpeg system call combines all the resultant videos into a single one that is moved to ~/storage/shared. If seventh argument is too big, it maybe fill up your storage and makes the program crash, if that happens, you may delete all the frames in bmp_files manually with:
rm bmp_files/f*.bmp to solve the problem.

NOTE: You must have ffmpeg installed, if not and if you are in linux, type the command: 

sudo apt install ffmpeg

to install the tool, if you are using termux, sudo is not required:

apt install ffmpeg

NOTE 2: By weird reasons, the width of the grill must be a multiple of eight.

