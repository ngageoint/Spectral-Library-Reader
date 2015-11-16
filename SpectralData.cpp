#include "SpectralData.h"


SpectralData::SpectralData()
{
	//load records into recordTuples
	this->mUSGSDataRecord = USGS_SpectralDataReader();
	this->mUSGSDataRecord.cLoadRecords();
	this->aSetRecordListMap();
	this->aSetWavelengthData();
	this->mReducedLibrary = map<int, vector<int>>();
	this->aSetStatMoments();

	//setup some of the reduced library sets
	this->mReducedLibrary[0] = vector<int>();
	//the minerals that indicate copper
	this->mReducedLibrary[0].push_back(162); 
	this->mReducedLibrary[0].push_back(489);
	this->mReducedLibrary[0].push_back(115);
	this->mReducedLibrary[0].push_back(238);

}

SpectralData::~SpectralData()
{
}

SpectralData::recordTuple	SpectralData::aGetSpectralRecord(int RecordIndex){
	//this method gets the full spectral data for spectral index "RecordIndex" concatenating library records if need be
	recordTuple rvalue = recordTuple();
	rvalue.id = -1;
	
	int lIndex = this->aGetRecordListElement(RecordIndex + 10);												// the first 10 records are header data
	int lContinuationCount = 0;																			//counts the number of continutaion records we have processed
	USGS_SpectralDataReader::USGSRecord lRecord = this->mUSGSDataRecord.aGetRecordUSGSFormat(lIndex);	//retrieves the record that starts the spectral data
	int lWavelengthID = lRecord.irwav;

	//check to seee if the record is an error record
	string lErrorCheck = string((char *)lRecord.ititl);													//check to see if this record is an error record
	int lErrorFound = lErrorCheck.find("error");
	
	//if no error and the index is good
	if (lRecord.irwav >= 6 && lErrorFound <0 && RecordIndex >= 0 && RecordIndex < this->mRecordList.size()){
		
		do{
	
		//now we parse the data records
		if (!lRecord.icflag[0] && !lRecord.icflag[1]){
			//flag indicates that record is the start of a data set
			rvalue = recordTuple();
			rvalue.id = lIndex;
			std::memcpy(rvalue.title, lRecord.ititl, 40);
			rvalue.channels = lRecord.itchan;
			std::memcpy(&(rvalue.reflectances[0]), lRecord.data, 1024);
			lContinuationCount = 0;
		}

		if (lRecord.icflag[0] && !lRecord.icflag[1]){
			//flag indicates that record is a continution data set
			int lCurrentCopyIndex = 256 + lContinuationCount * 383;
			if (lCurrentCopyIndex + 383 <= rvalue.channels){
				std::memcpy(&(rvalue.reflectances[lCurrentCopyIndex]), lRecord.cdata, 1532);
			}
			else{
				std::memcpy(&(rvalue.reflectances[lCurrentCopyIndex]), lRecord.cdata, 4 * (rvalue.channels - lCurrentCopyIndex));
			}

			lContinuationCount++;
		}

		lRecord = this->mUSGSDataRecord.aGetRecordUSGSFormat(lIndex + lContinuationCount + 1);

		} while (lRecord.icflag[0]); //keep processing if the record is a continuation record

		//retrieve wavelength data for this dataset
		int wavelengthIndex = 0;
		for (int i = 0; i < this->mWavelengthsData.size();i++){
			if (this->mWavelengthsData[i].id == lWavelengthID){
				wavelengthIndex = i;
				i = this->mWavelengthsData.size();
			}
		}

		//load reflectance data zeroing out negative values
		for (int i = 0; i < lRecord.itchan; i++){
			rvalue.wavelengths[i] = this->mWavelengthsData[wavelengthIndex].reflectances[i]; //wavelength data was stuck in the reflectances array
			if (rvalue.reflectances[i] < 0.0) { 
				rvalue.reflectances[i] = 0.0; 
			}
		}
		
		
	}

	//return the recordTuple
	return rvalue;
}

int	SpectralData::aGetRecordListElement(int index){
	//this method takes in a spectral index and returns a library index
	int rvalue = std::numeric_limits<int>::quiet_NaN();
	if (index >= 0 && index < this->mRecordList.size()){
		rvalue = this->mRecordList[index];
	}
	return rvalue;
}

SpectralData::WV3Spectra SpectralData::aGetWV3Record(int RecordIndex){
	//this method gets a spectral record, simulates what it would have looked like collected with WV3, and then returns a WV3 Spectra struct
	SpectralData::WV3Spectra rvalue;
	rvalue.USGSBase = this->aGetSpectralRecord(RecordIndex);
	
	if (rvalue.USGSBase.id >= 0){
		//id = -1 is used to indicate a record that we consider unusuable
		for (int i = 0; i < 8; i++){
			rvalue.SimulatedBands[i] = 0.0;
		}

		float lCurrentWavelength = 0.0;
		//go through the record channels and integrate the responses. Within the bands
		//the records record values in microns thus the 1.195 to 2.365 magic numbers
		// we consider the band efficiencies to all be equal whic isn't horrible given WV3's
		//spectral responses, bu this could be improved y using the actual reflectance values
		for (int i = 0; i < rvalue.USGSBase.channels; i++){
			lCurrentWavelength = rvalue.USGSBase.wavelengths[i];

			if (lCurrentWavelength >= 1.195 && lCurrentWavelength <= 1.225){
				rvalue.SimulatedBands[0] += rvalue.USGSBase.reflectances[i];
			}
			if (lCurrentWavelength >= 1.550 && lCurrentWavelength <= 1.590){
				rvalue.SimulatedBands[1] += rvalue.USGSBase.reflectances[i];
			}
			if (lCurrentWavelength >= 1.640 && lCurrentWavelength <= 1.680){
				rvalue.SimulatedBands[2] += rvalue.USGSBase.reflectances[i];
			}
			if (lCurrentWavelength >= 1.710 && lCurrentWavelength <= 1.750){
				rvalue.SimulatedBands[3] += rvalue.USGSBase.reflectances[i];
			}

			if (lCurrentWavelength >= 2.145 && lCurrentWavelength <= 2.185){
				rvalue.SimulatedBands[4] += rvalue.USGSBase.reflectances[i];
			}
			if (lCurrentWavelength > 2.185 && lCurrentWavelength <= 2.225){
				rvalue.SimulatedBands[5] += rvalue.USGSBase.reflectances[i];
			}
			if (lCurrentWavelength >= 2.235 && lCurrentWavelength <= 2.285){
				rvalue.SimulatedBands[6] += rvalue.USGSBase.reflectances[i];
			}
			if (lCurrentWavelength >= 2.295 && lCurrentWavelength <= 2.365){
				rvalue.SimulatedBands[7] += rvalue.USGSBase.reflectances[i];
			}
		}
		
		//normalize band data
		float total = 0.0;
		for (int i = 0; i < 8; i++){
			total += rvalue.SimulatedBands[i] * rvalue.SimulatedBands[i];
		}

		total = sqrt(total);
		
		for (int i = 0; i < 8; i++){
			rvalue.SimulatedBands[i] /= total;
		}

	}
	else{
		//bad data so null out the bands
		for (int i = 0; i < 8;i++){
			rvalue.SimulatedBands[i] = 0.0;
		}
	}
	//return WV3 spectra struct
	return rvalue;
}

SpectralData::WV3Spectra SpectralData::aGetStatNormedWV3Record(int RecordIndex){
	//this method gets a spectral record, simulates what it would have looked like collected with WV3, and then returns a WV3 Spectra struct
	SpectralData::WV3Spectra rvalue;
	rvalue.USGSBase = this->aGetSpectralRecord(RecordIndex);

	if (rvalue.USGSBase.id >= 0){
		//id = -1 is used to indicate a record that we consider unusuable
		for (int i = 0; i < 8; i++){
			rvalue.SimulatedBands[i] = 0.0;
		}

		float lCurrentWavelength = 0.0;
		//go through the record channels and integrate the responses. Within the bands
		//the records record values in microns thus the 1.195 to 2.365 magic numbers
		for (int i = 0; i < rvalue.USGSBase.channels; i++){
			lCurrentWavelength = rvalue.USGSBase.wavelengths[i];

			if (lCurrentWavelength >= 1.195 && lCurrentWavelength <= 1.225){
				rvalue.SimulatedBands[0] += rvalue.USGSBase.reflectances[i];
			}
			if (lCurrentWavelength >= 1.550 && lCurrentWavelength <= 1.590){
				rvalue.SimulatedBands[1] += rvalue.USGSBase.reflectances[i];
			}
			if (lCurrentWavelength >= 1.640 && lCurrentWavelength <= 1.680){
				rvalue.SimulatedBands[2] += rvalue.USGSBase.reflectances[i];
			}
			if (lCurrentWavelength >= 1.710 && lCurrentWavelength <= 1.750){
				rvalue.SimulatedBands[3] += rvalue.USGSBase.reflectances[i];
			}

			if (lCurrentWavelength >= 2.145 && lCurrentWavelength <= 2.185){
				rvalue.SimulatedBands[4] += rvalue.USGSBase.reflectances[i];
			}
			if (lCurrentWavelength > 2.185 && lCurrentWavelength <= 2.225){
				rvalue.SimulatedBands[5] += rvalue.USGSBase.reflectances[i];
			}
			if (lCurrentWavelength >= 2.235 && lCurrentWavelength <= 2.285){
				rvalue.SimulatedBands[6] += rvalue.USGSBase.reflectances[i];
			}
			if (lCurrentWavelength >= 2.295 && lCurrentWavelength <= 2.365){
				rvalue.SimulatedBands[7] += rvalue.USGSBase.reflectances[i];
			}
		}

		//normalize band data
		float total = 0.0;
		for (int i = 0; i < 8; i++){
			total += rvalue.SimulatedBands[i] * rvalue.SimulatedBands[i];
		}
		total = sqrt(total);
		for (int i = 0; i < 8; i++){
			rvalue.SimulatedBands[i] /= total;
		}

	}
	else{
		//bad data so null out the bands
		for (int i = 0; i < 8; i++){
			rvalue.SimulatedBands[i] = 0.0;
		}
	}

	//move data into a statistically normalized frame by subtractig mean and placing data into a distribution with unit variance per dimension
	for (int i = 0; i < numBands;i++){
		rvalue.SimulatedBands[i] -= this->mStatMoments[0].SimulatedBands[i];//(x - <x>)
		rvalue.SimulatedBands[i] /= sqrt(this->mStatMoments[1].SimulatedBands[i]);//(x - <x>) / var(x)

	}

	//return WV3 spectra struct
	return rvalue;
}

int	SpectralData::aGetNumSpectralRecords(){
	//returns the number of spectral reflectance records the class has access to
	int rvalue = this->mRecordList.size();
	return rvalue;
}

 map<int, vector<int>>		SpectralData::aGetReducedLibrary(){
	 //returns the reduced library for selective comparisons
	map<int, vector<int>>	 rvalue;
	rvalue = this->mReducedLibrary;
	return rvalue;
}
 float *	SpectralData::aGetMoment(int index){
	 //returns either the mean or standard deviation
	 float * rvalue = nullptr;
	 if (index < 2 && index >=0){
		 rvalue = &(this->mStatMoments[index].SimulatedBands[0]);
	 }
	 return rvalue;
 }
bool SpectralData::aSetWavelengthData(){
	//uploads the wavelength data to the class instance
	bool rvalue = false;

	USGS_SpectralDataReader::USGSRecord lRecord;
	int lCurrentIndex = 0;
	int lContinuationCount = 0;

	// the 77th record is where spectral data starts so we end there
	for (int i = 0; i < 77; i++){
		//grab the record
		lRecord = this->mUSGSDataRecord.aGetRecordUSGSFormat(i);

		//only grab non-text data
		if (lRecord.icflag[1]){
			//if this flag is true then the data is text and we will skip it for data load
			continue;
		}

		//now we parse the data records
		if (!lRecord.icflag[0]){
			//flag indicates that record is the start of a data set
			mWavelengthsData[i] = recordTuple();
			mWavelengthsData[i].id = i;
			std::memcpy(mWavelengthsData[i].title, lRecord.ititl, 40);
			mWavelengthsData[i].channels = lRecord.itchan;
			std::memcpy(mWavelengthsData[i].reflectances, lRecord.data, 1024); // we are loading the data for wavelength into the reflectance array for consistency with the way other records are loaded

			lCurrentIndex = i;
			lContinuationCount = 0;
		}
		else{
			//flag indicates that record is a continution a data set
			int lCurrentCopyIndex = 256 + lContinuationCount * 383;
			
			if (lCurrentCopyIndex + 383 <= mWavelengthsData[lCurrentIndex].channels){
				std::memcpy(&(mWavelengthsData[lCurrentIndex].reflectances[lCurrentCopyIndex]), lRecord.cdata, 1532);
			}else{
				std::memcpy(&(mWavelengthsData[lCurrentIndex].reflectances[lCurrentCopyIndex]), lRecord.cdata, 4 * (mWavelengthsData[lCurrentIndex].channels - lCurrentCopyIndex));
			}

			lContinuationCount++;

		}
		
	}

	float reflect = 0;
	for (int i = 0; i < mWavelengthsData.size(); i++){
		for (int j = 0; j < 5000;j++){
			reflect = mWavelengthsData[i].reflectances[j];
			if ( reflect > 10.0 || reflect < 0.0){
				mWavelengthsData[i].reflectances[j] = 0;
				break;
			}
		}
	}
	rvalue = true;
	
	return rvalue;
}

bool SpectralData::aSetRecordListMap(){
	//this method maps spectral indices (the records we generally want for comparison) 
	//to library indices (what is actually in the file)
	bool rvalue = false;

	USGS_SpectralDataReader::USGSRecord lRecord;
	int lCurrentIndex = 0;
	int lContinuationCount = 0;
	int lRecordID = 0;
	
	for (int i = 0; i < this->mUSGSDataRecord.aGetNumRecords(); i++){
		//grab the record
		lRecord = this->mUSGSDataRecord.aGetRecordUSGSFormat(i);

		//only grab non-text data
		if (lRecord.icflag[1]){
			//if this flag is true then the data is text and we will skip it for data load
			continue;
		}

		
		if (!lRecord.icflag[0]){
			//flag indicates that record is the start of a data set
			this->mRecordList[lRecordID] = i;
			lRecordID++;
		}
	}

	return rvalue;
}

bool SpectralData::aSetStatMoments(){
	bool rvalue = false;
	WV3Spectra lCurrentSpectra;
	WV3Spectra lCurrentSpectraMean;
	WV3Spectra lCurrentSpectra2edMean;
	WV3Spectra lCurrentSpectraMean2ed;

	for (int j = 0; j < numBands; j++){
		lCurrentSpectraMean.SimulatedBands[j] = 0.0;
		lCurrentSpectra2edMean.SimulatedBands[j] = 0.0;
		lCurrentSpectraMean2ed.SimulatedBands[j] = 0.0;
	}
	//aggregate data
	for (int i = 0; i < this->aGetNumSpectralRecords(); i++){
		lCurrentSpectra = this->aGetWV3Record(i);
		for (int j = 0; j < numBands;j++){
			//x
			lCurrentSpectraMean.SimulatedBands[j] += lCurrentSpectra.SimulatedBands[j];
			//x^2
			lCurrentSpectra2edMean.SimulatedBands[j] += lCurrentSpectra.SimulatedBands[j] * lCurrentSpectra.SimulatedBands[j];

		}
	}

	//divide by the number of datapoints
	for (int j = 0; j < numBands; j++){
		//<x>
		lCurrentSpectraMean.SimulatedBands[j] /= this->aGetNumSpectralRecords();
		//<x^2>
		lCurrentSpectra2edMean.SimulatedBands[j] /= this->aGetNumSpectralRecords();
	}

	//assign the mean
	this->mStatMoments[0] = lCurrentSpectraMean;

	//calculate the mean squared and the variance
	for (int j = 0; j < numBands; j++){
		//<x><x> = <x>^2
		lCurrentSpectraMean2ed.SimulatedBands[j] = lCurrentSpectraMean.SimulatedBands[j] * lCurrentSpectraMean.SimulatedBands[j];
		//var(x) = <x^2> - <x>^2
		this->mStatMoments[1].SimulatedBands[j] = lCurrentSpectra2edMean.SimulatedBands[j] - lCurrentSpectraMean2ed.SimulatedBands[j];
	}

	return rvalue;
}