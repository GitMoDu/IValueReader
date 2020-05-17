// IReaderTask.h

#ifndef _IREADER_TASK_h
#define _IREADER_TASK_h

#include <IValueReader.h>

class IReaderTask
{
public:
	IReaderTask() {}

	virtual bool AddValueReader(IValueReader* valueReader) {  }
	virtual void StartReading() {}
	virtual void StopReading() {}
	virtual uint8_t GetReaderCount() { return 0; }

//#ifdef DEBUG_VALUE_READER
//	//uint32_t GetReadingDuration() { return 0; }
//#endif
};
#endif