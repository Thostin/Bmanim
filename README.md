# Bmanim

Just modify the arguments of the function call in main to create the frames of the resultant video; then use this ffmpeg command to link all those into a video:

(You must be in the bmp_files folder to execute this).

ffmpeg -r 30 -f image2 -s 1920x1080 -i f%d.bmp -vcodec libx264 -crf 1  -pix_fmt yuv420p langton.mp4

The -r 30 argument says that the video is at 30fps, can be modified; -crf 1 especifies level of compression, the lowest it is, better quality: usually 15-25 is fine. langton.mp4 is the name of the output video, can be modified.

Note: The func.c file is completely unusefull, but there are some weird functions there.
