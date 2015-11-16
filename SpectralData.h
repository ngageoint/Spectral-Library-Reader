/*
This class uses USGS_SpectralDataReader to read the splib06a spectral reflectance library, and return an object that both 
concatenates related records and simulates the normalized response for  Digital Globe's World View 3 sensor observing the 
spectral sample.

Misc Information:
since a spectral sample's recording can span a number of the library data records (which are byte length fixed, but luckily sequential),
the mRecordList map is used to translate a spectral sample index to the library's initial record index for the spectral sample.

*/

#pragma once
#include "USGS_SpectralDataReader.h"
#include <map>
#include <vector>
#include <string.h>

class SpectralData
{
	
public:
	static const int numBands = 8;

	struct recordTuple{										//structure holds a set of concatenated records that all belong to the same spectral sample
		unsigned char	title[40];							//Title taken straight from Binary Data
		int				id;									//id taken straight from Binary Data
		int				channels;							//number of channels in this spectral set
		float 			reflectances[5000];					//values for the concatenated record
		float 			wavelengths[5000];					//wavelengths for the concatenated records
	};

	struct WV3Spectra{
		recordTuple		USGSBase;							//spectral sample records
		float			SimulatedBands[numBands];			//a normalized version of the USGSBase spectral relfectance data integrated over the bandpass properties for the WV3 SWIR bands
	};

private:

	USGS_SpectralDataReader				mUSGSDataRecord;	//reads in speclib06a and returns data as requested
	map<int, recordTuple>				mWavelengthsData;	//holds the hand full of wavelength data references. the map key is the record index where the wavelength data can be found in the library  
	map<int, int>						mRecordList;		//map key is the index of the spectral dataset of interest, map value is the record index where the dataset starts in the library
	map<int, vector<int>>				mReducedLibrary;	//holds a list of the values to use for reduced library search
	WV3Spectra							mStatMoments[2];	

public:
	SpectralData();
	~SpectralData();

	//////////////////
	//Accessor Methods
	//////////////////

	/////////////////////
	//get methods
	/////////////////////
	recordTuple							aGetSpectralRecord(int SpectralRecordIndex);		//gets the spectral data for spectral record "RecordIndex"
	int									aGetRecordListElement(int SpectralRecordIndex);		//converst spectral index to library record index
	WV3Spectra							aGetWV3Record(int SpectralRecordIndex);				//gets the spectral data for the spectral record "RecordIndex" and then simulates WV3 SWIR responses
	WV3Spectra							aGetStatNormedWV3Record(int SpectralRecordIndex);	//gets the spectral data for the spectral record "RecordIndex" and then simulates WV3 SWIR responses
	int									aGetNumSpectralRecords();							//gets the number of spectral records
	map<int, vector<int>>				aGetReducedLibrary();
	float *								aGetMoment(int index);
	
	/////////////////////
	//set methods
	/////////////////////
	bool								aSetWavelengthData();						//loads the mWavelengthsData map
	bool								aSetRecordListMap();						//loads the mRecordsList map
	bool								aSetStatMoments();							//generates the moments of the data

};

