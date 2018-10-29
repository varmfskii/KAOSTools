# KAOSTools

Tools for KAOS based games on the Color Computer 3

* MapConverter - Converts Tiled .tmx and generates 6809 assembler code compatible with LWTools.

Usage:

`
	mapcoverter [options] filename
`


Options:

|Option					| Description
|-----------------------|--------------
| output | Specifies the directory where the assembler files will be written to.
| objectlist | Specifies the filename of the Object name to object id conversion list. Only necessary if the map contains an object group layer.
| emptyid | Specifies the tile id to use for empty space in the map (defaults to 0).

Example:

`
	mapconverter --emptyid=0 --output=../mapsincode --objectlist=maps/common.xml maps/w1l1.tmx`
`

