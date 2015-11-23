# Spectral-Library-Reader
C++ Library that reads the splib06a file - a custom binary spectral reflectance database file created by USGS. Robust documentation can be found in the documentation files in this repo as well as inline with the source code.

When splib06a is read, it provides all the information you need to graph out the spectral reflectances of the materials in the file. An example provided by USGS at (http://speclab.cr.usgs.gov/spectral.lib06/ds231/datatable.html) is shown below.

![Acmite Graph](https://github.com/ngageoint/Spectral-Library-Reader/blob/master/Acemite_Graph.gif)

You would use this class and the splib06a file if you had spectral reflectance data - for example multiband imagery of a mine in Chile - and you wanted to understand the presence or absence of particulair materials in the image (copper bearing minerals for instance) 

![Chile Mine](https://github.com/ngageoint/Spectral-Library-Reader/blob/master/ChileMine.png)
