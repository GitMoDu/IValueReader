// IValueReader.h

#ifndef _IVALUE_READER_h
#define _IVALUE_READER_h

#include <stdint.h>

class IValueReader
{
public:
	// Read value, return true when done.
	// If false is returned, the delay until next Read() can be set.
	virtual bool Read(uint32_t& delay) { return true; }

	// Clear the stored value.
	virtual void Clear() { }

	// Stop any pending operations and turn off used hardware.
	virtual void Stop() { }
};
#endif