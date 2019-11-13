0 0


_
(row col)
To add a level to a set, increase the col value.
To add a new set, add a new row one greater than the previous.

Then, add the level with filename [row]_[col].txt

Levels are in the following format:
[operators]

[color1]

[color2]

[solution grid]
>
[grid0]
>
[grid1]
>
[grid2]
>
.
.
.
>
___
[operators] should be a series of characters, separated by spaces:
+ plus
- minus
f flip
b bucket
___
A color is a series of 4 values, 0-255, representing RGBA
Separate these numbers with spaces.

___
Here's an example 3x3 grid:

010
202
010

Make sure to place a '>' after each grid
___
Complete Level example:

f + + - b

255 255 255 255
45 125 249 255

02100
00002
10101
20000
00120
>
10011
10000
00000
00001
11001
>
00200
00200
11111
00200
00200
>
01000
00001
11111
10000
00010
>
00000
01110
01110
01110
00000
>
