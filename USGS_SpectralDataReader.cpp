#include "USGS_SpectralDataReader.h"

USGS_SpectralDataReader::USGS_SpectralDataReader()
{
}

USGS_SpectralDataReader::~USGS_SpectralDataReader()
{
}

char * USGS_SpectralDataReader::aGetRecordBinaryData(int index){
	//returns a pointer to the "index" record
	char * rvalue = nullptr;

	if (index >= 0 && index < this->mRecords.size()){
		rvalue = this->mRecords[index];
	}
	return rvalue;
}

USGS_SpectralDataReader::USGSRecord	USGS_SpectralDataReader::aGetRecordUSGSFormat(int index) {
	//get the "index" record and parse it into the USGS (Specpr) Format
	USGS_SpectralDataReader::USGSRecord rvalue = USGS_SpectralDataReader::USGSRecord();
	
	if (index >= 0 && index < this->mRecords.size()){
		char * baseptr = this->aGetRecordBinaryData(index);
		int offset = 0;
		rvalue.icflag[0] = ((int *)baseptr)[offset] & 0x01000000;
		rvalue.icflag[1] = ((int *)baseptr)[offset] & 0x02000000;
		rvalue.icflag[2] = ((int *)baseptr)[offset] & 0x04000000;
		rvalue.icflag[3] = ((int *)baseptr)[offset] & 0x08000000;
		rvalue.icflag[4] = ((int *)baseptr)[offset] & 0x10000000;
		rvalue.icflag[5] = ((int *)baseptr)[offset] & 0x20000000;
		//the rest of the icflags aren't currently used by USGS

		if (!rvalue.icflag[0] && !rvalue.icflag[1]){
			//if this is not a continuation record and not a text record we process like so ...	
			offset += 4;
			std::memcpy(&(rvalue.ititl), &(baseptr[offset]), 40);
			offset += 40;
			std::memcpy(&(rvalue.usernm), &(baseptr[offset]), 8);
			offset += 8;
			std::memcpy(&(rvalue.iscta), &(baseptr[offset]), 4);
			this->cIntelByteFlip((unsigned char*)&(rvalue.iscta), 4);
			offset += 4;
			std::memcpy(&(rvalue.isctb), &(baseptr[offset]), 4);
			this->cIntelByteFlip((unsigned char*)&(rvalue.isctb), 4);
			offset += 4;
			std::memcpy(&(rvalue.jdatea), &(baseptr[offset]), 4);
			this->cIntelByteFlip((unsigned char*)&(rvalue.jdatea), 4);
			offset += 4;
			std::memcpy(&(rvalue.jdateb), &(baseptr[offset]), 4);
			this->cIntelByteFlip((unsigned char*)&(rvalue.jdateb), 4);
			offset += 4;
			std::memcpy(&(rvalue.istb), &(baseptr[offset]), 4);
			this->cIntelByteFlip((unsigned char*)&(rvalue.istb), 4);
			offset += 4;
			std::memcpy(&(rvalue.isra), &(baseptr[offset]), 4);
			this->cIntelByteFlip((unsigned char*)&(rvalue.isra), 4);
			offset += 4;
			std::memcpy(&(rvalue.isdec), &(baseptr[offset]), 4);
			this->cIntelByteFlip((unsigned char*)&(rvalue.isdec), 4);
			offset += 4;
			std::memcpy(&(rvalue.itchan), &(baseptr[offset]), 4);
			this->cIntelByteFlip((unsigned char*)&(rvalue.itchan), 4);
			offset += 4;
			std::memcpy(&(rvalue.irmas), &(baseptr[offset]), 4);
			this->cIntelByteFlip((unsigned char*)&(rvalue.irmas), 4);
			offset += 4;
			std::memcpy(&(rvalue.revs), &(baseptr[offset]), 4);
			this->cIntelByteFlip((unsigned char*)&(rvalue.revs), 4);
			offset += 4;
			std::memcpy(&(rvalue.iband[0]), &(baseptr[offset]), 8);
			this->cIntelByteFlip((unsigned char*)&(rvalue.iband[0]), 4);
			this->cIntelByteFlip((unsigned char*)&(rvalue.iband[1]), 4);
			offset += 8;
			std::memcpy(&(rvalue.irwav), &(baseptr[offset]), 4);
			this->cIntelByteFlip((unsigned char*)&(rvalue.irwav), 4);
			offset += 4;
			std::memcpy(&(rvalue.irespt), &(baseptr[offset]), 4);
			this->cIntelByteFlip((unsigned char*)&(rvalue.irespt), 4);
			offset += 4;
			std::memcpy(&(rvalue.irecno), &(baseptr[offset]), 4);
			this->cIntelByteFlip((unsigned char*)&(rvalue.irecno), 4);
			offset += 4;
			std::memcpy(&(rvalue.itpntr), &(baseptr[offset]), 4);
			this->cIntelByteFlip((unsigned char*)&(rvalue.itpntr), 4);
			offset += 4;
			std::memcpy(&(rvalue.ihist), &(baseptr[offset]), 60);
			offset += 60;
			std::memcpy(&(rvalue.mhist), &(baseptr[offset]), 296);
			offset += 296;
			std::memcpy(&(rvalue.nruns), &(baseptr[offset]), 4);
			this->cIntelByteFlip((unsigned char*)&(rvalue.nruns), 4);
			offset += 4;
			std::memcpy(&(rvalue.siangl), &(baseptr[offset]), 4);
			this->cIntelByteFlip((unsigned char*)&(rvalue.siangl), 4);
			offset += 4;
			std::memcpy(&(rvalue.seangl), &(baseptr[offset]), 4);
			this->cIntelByteFlip((unsigned char*)&(rvalue.seangl), 4);
			offset += 4;
			std::memcpy(&(rvalue.sphase), &(baseptr[offset]), 4);
			this->cIntelByteFlip((unsigned char*)&(rvalue.sphase), 4);
			offset += 4;
			std::memcpy(&(rvalue.iwtrns), &(baseptr[offset]), 4);
			this->cIntelByteFlip((unsigned char*)&(rvalue.iwtrns), 4);
			offset += 4;
			std::memcpy(&(rvalue.itimch), &(baseptr[offset]), 4);
			this->cIntelByteFlip((unsigned char*)&(rvalue.itimch), 4);
			offset += 4;
			std::memcpy(&(rvalue.xnrm), &(baseptr[offset]), 4);
			this->cIntelByteFlip((unsigned char*)&(rvalue.xnrm), 4);
			offset += 4;
			std::memcpy(&(rvalue.scatim), &(baseptr[offset]), 4);
			this->cIntelByteFlip((unsigned char*)&(rvalue.scatim), 4);
			offset += 4;
			std::memcpy(&(rvalue.timint), &(baseptr[offset]), 4);
			this->cIntelByteFlip((unsigned char*)&(rvalue.timint), 4);
			offset += 4;
			std::memcpy(&(rvalue.tempd), &(baseptr[offset]), 4);
			this->cIntelByteFlip((unsigned char*)&(rvalue.tempd), 4);
			offset += 4;
			std::memcpy(&(rvalue.data), &(baseptr[offset]), 1024);
			offset += 1024;
			for (int i = 0; i < 256; i++){
				this->cIntelByteFlip((unsigned char*)&(rvalue.data[i]), 4);
			}
			offset = 0;

		}

		offset = 0;
		
		if (rvalue.icflag[0] && ! rvalue.icflag[1]){
			//if this is a continuation record but not a text record

			offset += 4;//flag bytes that should be skipped
			std::memcpy(&(rvalue.cdata), &(baseptr[offset]), 1532);
			offset += 1532;
			for (int i = 0; i < 383; i++){
				this->cIntelByteFlip((unsigned char*)&(rvalue.cdata[i]), 4);
			}
		}

		offset = 0;

		if (!rvalue.icflag[0] && rvalue.icflag[1]){
			//record is not a continuation record but is a text record
			offset += 4;//flag bytes
			std::memcpy(&(rvalue.ititl), &(baseptr[offset]), 40);
			offset += 40;
			std::memcpy(&(rvalue.usernm), &(baseptr[offset]), 8);
			offset += 8;
			std::memcpy(&(rvalue.itxtpt), &(baseptr[offset]), 4);
			this->cIntelByteFlip((unsigned char*)&(rvalue.itxtpt), 4);
			offset += 4;
			std::memcpy(&(rvalue.itxtch), &(baseptr[offset]), 4);
			this->cIntelByteFlip((unsigned char*)&(rvalue.itxtch), 4);
			offset += 4;
			std::memcpy(&(rvalue.itext), &(baseptr[offset]), 1476);
			offset += 1476;
		}

		offset = 0;

		if (rvalue.icflag[0] && rvalue.icflag[1]){
			//record is a continuation record and a text record
			offset = 4;
			std::memcpy(&(rvalue.tdata), &(baseptr[offset]), 1532);
			offset += 1532;

		}

	}

	//return parsed record
	return rvalue;
}

int USGS_SpectralDataReader::aGetNumRecords(){
	//get total number of records in the library
	int rvalue = std::numeric_limits<int>::quiet_NaN();
	rvalue = this->mRecords.size();
	return rvalue;
}

int	USGS_SpectralDataReader::aGetNumCompiledRecords(){
	//get number of records that ar not continuation and not text
	int rvalue = 0;
	for (int i = 0; i < this->mRecords.size();i++){
		char * baseptr = this->mRecords[i];

		if (!(((int *)baseptr)[0] & 0x01000000) &&
			!(((int *)baseptr)[0] & 0x02000000)){
			//start of new spectral reflectance record
			rvalue++;
		}
	}

	return rvalue;
}
 
bool USGS_SpectralDataReader::aCreateRecord(){
	//allocate heap memory for a record and push the pointer to the data onto the mRecords vector
	bool rvalue = false;
	this->mRecords.push_back(new  char[this->sRecordLength]);
	return rvalue;
}

bool USGS_SpectralDataReader::cIntelByteFlip(unsigned char * data, int bytes){
	//flip the byte order if you are on an intel based system
	bool rvalue = false;
	unsigned int lSwapped = 0xffffffff;// 4 bytes
	
	switch (bytes)
	{
	case 4:
		lSwapped =	(lSwapped & ((int)data[3] << 0))		|	// move byte 3 to byte 0
					(lSwapped & ((int)data[2] << 8))		|	// move byte 1 to byte 2
					(lSwapped & ((int)data[1] << 16))		|	// move byte 2 to byte 1
					(lSwapped & ((int)data[0] << 24));			// byte 0 to byte 3
		
		std::memcpy(data,&lSwapped,4);
		break;
	default:
		break;
	}
	return rvalue;
}

bool USGS_SpectralDataReader::cLoadRecords(){
	//open the Spectral Data Library and read the records into the class instance
	bool rvalue = false;
	int lRecordsCounter = 0;
	int lFileSize = 0;
	
	streampos begin, end;
	ifstream lSpecFile;
	lSpecFile.open("SpectralData/splib06a", ios::binary);
	
	if (lSpecFile.is_open())
	{
		begin = lSpecFile.tellg();
		lSpecFile.seekg(0, ios::end);
		end = lSpecFile.tellg();
		lFileSize = end - begin;

		while (lRecordsCounter * this->sRecordLength < lFileSize){
			this->aCreateRecord();
			
			lSpecFile.seekg(lRecordsCounter * this->sRecordLength, ios::beg);
			lSpecFile.read(this->aGetRecordBinaryData(this->mRecords.size()-1), this->sRecordLength);

			lRecordsCounter++;
		}
		

		lSpecFile.close();
	}

	return rvalue;

}