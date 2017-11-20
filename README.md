# probefit
LinuxCNC PROBE-RESULTS.TXT to Viacad grid surface conversion


ABOUT

Probefit was made for the specific situation when we have one PROBE-RESULTS.TXT file generated by LinuxCNC from one probed surface and want to open that file as a surface on Viacad 3d program with import grid surface.

The following link points to one discussion on Viacad forum, there Tim Olson share one .pdf document where there are the description for necessary format in Viacad import points cloud text file:
http://forum.punchcad.com/default.aspx?g=posts&m=335#post335

![image][./image1.jpg]

OPTIONS

The program has the following options:


-m          metric units. Viacad by default will consider the points coordinates as inches, apart of units used by LinuxCNC when generating the file. In practice this option convert inches to milimeters, dividing all input coordinates by 25.4.


-x          this option has to be followed by one number, floating point or not, which will be the multiplier of all input 'x' coordinates. 


-y          idem for 'y'.


-z          idem for 'z'.



SOURCE CODE

The source was coded using LCC-Win IDE (Windows) but use just standardized functions calls, so must compile without any flaw in many  operating systems. Apologize the messy.


IDEAS

As command line interface are something old (last millennia!!!) maybe someday I make one true MS-Windows version. Maybe...


AUTHOR

Constructive criticism and sugestions are always welcome (please note that English is not my daily language):
rymaedaATyahoo.com 
