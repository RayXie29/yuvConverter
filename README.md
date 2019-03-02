YUV format is a color encoded system, it can be transformed by RGB format.<br />
The concept of YUV format is to separate the color information and luminance information. The Y channel is for luminance value and U&V channels are for color value.
YUV format usually has smaller bandwidth than RGB format, because it reduced the chrominance information. But it can still perform good quality after decoding.

RGB to YUV conversion formula:<br />

Y = 0.299*R + 0.587*G + 0.114*B<br />
U = -0.169*R -0.331*G +0.5*B +128<br />
V = 0.5*R - 0.419*G - 0.081*B + 128<br />

There are two major types of YUV format <br />
1.planer: each Y, U and V put separately in the momory<br />
ex:<br />
YYYYY<br />
YYYYY<br />
UUUUU<br />
VVVVV<br />
2.semi-planer:Y and UV put separately in memory, the difference between planer and semi-planer is that UV format put together in semi-planer.<br />
ex:<br />
YYYYY<br />
YYYYY<br />
UVUVU<br />
VUVUV<br />


This repo include:<br />

RGB -> YUV444 planer<br />
RGB -> YUV444 semi-planer<br />
RGB -> YUV422 planer R<br />
RGB -> YUV422 planer YV16<br />
RGB -> YUV420 planer I420<br />
RGB -> YUV420 planer NV12<br />
RGB -> YUV420 semi-planer<br />

Also from above YUV format to RGB fomat<br />

