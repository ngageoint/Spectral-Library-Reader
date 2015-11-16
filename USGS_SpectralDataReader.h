/*
This class reads the USGS's spectral reflectance library (splib06a). It reads all records in the library, and then
provides a method to get a structure (USGSRecord) for the record that corresponds to the Specpr Digital Format found 
on the USGS website (http://speclab.cr.usgs.gov/specpr-format.html). There are a fair amount of "magic numbers" in 
the code that correspond to byte sizes as defined in the specpr format.

The code DOES NOT concatenate records. The records returned from either of the "get record" methods simply return
a 1536 byte long chunk of data (parsed if you ask for it with the GetUSGSFormat method)

Library Reference Format:
the records 0-6	 are header data for the library
the records 7-77 are  wavelength and resolution (in terms of FWHM bandwidth)
the rest of the records are spectral reflectance data

Author:
Clayton Davis
Lead Scientist / Data Scientist
Booz Allen Hamilton
*/

#pragma once
#include <iostream>
#include <fstream>
#include <limits>
#include <vector>

using namespace std;

class USGS_SpectralDataReader
{
public:
	struct USGSRecord{
		bool					icflag[32];
		unsigned char			ititl[40];
		unsigned char			usernm[8];
		unsigned int			iscta;
		unsigned int			isctb;
		unsigned int			jdatea;
		unsigned int			jdateb;
		unsigned int			istb;
		unsigned int			isra;
		unsigned int			isdec;
		unsigned int			itchan;
		unsigned int			irmas;
		unsigned int			revs;
		unsigned int			iband[2];
		unsigned int			irwav;
		unsigned int			irespt;
		unsigned int			irecno;
		unsigned int			itpntr;
		unsigned char			ihist[60];
		unsigned char			mhist[296];
		unsigned int			nruns;
		unsigned int			siangl;
		unsigned int			seangl;
		unsigned int			sphase;
		unsigned int			iwtrns;
		unsigned int			itimch;
		float					xnrm;
		float					scatim;
		float					timint;
		float					tempd;
		float					data[256];
		
		float					cdata[383];

		int						itxtpt;
		int						itxtch;
		unsigned char			itext[1476];

		unsigned char			tdata[1532];
	};

public:
	static const int					sRecordLength = 1536;	//Specpr data record size

private:
	vector<char * >						mRecords;				//holds pointers to USGS spectral data record

public:
	USGS_SpectralDataReader();
	~USGS_SpectralDataReader();

	//////////////////
	//Accessor Methods
	//////////////////

	//get methods
	char*								aGetRecordBinaryData(int index);	//returns pointer to a record that has been 
	USGSRecord							aGetRecordUSGSFormat(int index);	//returns a record parsed into the Specpr format
	int									aGetNumRecords();					//gets the overall number of records in the file
	int									aGetNumCompiledRecords();			//gets the number of spectral records
	
	//set methods
	bool								aCreateRecord();					//allocates head memory for record and pushes a pointer onto mRecords 

	///////////////////
	//Calculate Methods
	///////////////////
	
	bool								cIntelByteFlip(unsigned char * data, int bytes);	//changes the endian order of the data
	bool								cLoadRecords();										//loads all the records into mRecords
};

