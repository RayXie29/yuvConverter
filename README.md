YUV format is a color encoded system, it can be transformed by RGB format.__
The concept of YUV format is to separate the color information and luminance information. The Y channel is for luminance value and U&V channels are for color value.
YUV format usually has smaller bandwidth than RGB format, because it reduced the chrominance information. But it can still perform good quality after decoding.

RGB to YUV conversion formula:

Y = 0.299*R + 0.587*G + 0.114*B__
U = -0.169*R -0.331*G +0.5*B +128__
V = 0.5*R - 0.419*G - 0.081*B + 128__

There are two major types of YUV format__
1.planer: each Y, U and V put separately in the momory__
ex:__
YYYYY__
YYYYY__
UUUUU__
VVVVV__
2.semi-planer:Y and UV put separately in memory, the difference between planer and semi-planer is that UV format put together in semi-planer.__
ex:__
YYYYY__
YYYYY__
UVUVU__
VUVUV__


This repo include:__

RGB -> YUV444 planer__
RGB -> YUV444 semi-planer__
RGB -> YUV422 planer R__
RGB -> YUV422 planer YV16__
RGB -> YUV420 planer I420__
RGB -> YUV420 planer NV12__
RGB -> YUV420 semi-planer__

Also from above YUV format to RGB fomat__

