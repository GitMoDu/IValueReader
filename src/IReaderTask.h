// IReaderTask.h

#ifndef _IREADER_TASK_h
#define _IREADER_TASK_h

#include <IValueReader.h>

class IReaderTask
{
public:
	virtual bool AddValueReader(IValueReader* valueReader) { return false; }
	virtual void StartReading() {}
	virtual void StopReading() {}
};
#endif