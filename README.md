# Spectral-Library-Reader
C++ Library that reads the splib06a file - a custom binary spectral reflectance database file created by USGS. Robust documentation can be found in the documentation files in this repo as well as inline with the source code.

When splib06a is read, it provides all the information you need to graph out the spectral reflectances of the materials in the file. An example provided by USGS at (http://speclab.cr.usgs.gov/spectral.lib06/ds231/datatable.html) is shown below.

![Acmite Graph](https://github.com/ngageoint/Spectral-Library-Reader/blob/master/Acemite_Graph.gif)

You would use this class and the splib06a file if you had spectral reflectance data - for example multiband imagery of a mine in Chile - and you wanted to understand the presence or absence of particular materials in the image (copper bearing minerals for instance). 

![Chile Mine](https://github.com/ngageoint/Spectral-Library-Reader/blob/master/ChileMine.png)


##Origin
The Spectral Library Reader software was developed at the National Geospatial-Intelligence Agency (NGA) in collaboration with Booz Allen. The government has "unlimited rights" and is releasing this software to increase the impact of government investments by providing developers with the opportunity to take things in new directions. The software use, modification, and distribution rights are stipulated within the [MIT] (http://choosealicense.com/licenses/mit/) license.  

###Pull Requests
If you'd like to contribute to this project, please make a pull request. We'll review the pull request and discuss the changes. All pull request contributions to this project will be released under the MIT license.  

Software source code previously released under an open source license and then modified by NGA staff is considered a "joint work" (see 17 USC § 101); it is partially copyrighted, partially public domain, and as a whole is protected by the copyrights of the non-government authors and must be released according to the terms of the original open source license.
