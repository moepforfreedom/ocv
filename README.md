* upsampvid.exe: command-line tool to upsample the frame rate of a video file.  The command line args are as follows
  * -i [input video]
  * -o [output avi file]
  * -s [start frame]
  * -n [number of inbetween frames]
  * -x [scale factor % for output (1-100, will be rounded to the nearest 5%)]
  * -f [flow algorithm to use]
    * N: no interpolation
    * S: simple flow
    * D: deep flow, best
    * I: DISFlow, fastest

