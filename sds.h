#include <iostream>
#include <cstring>
#include <stdexcept>
#include <cassert>



#define SDS_TYPE_5 0
#define SDS_TYPE_8 1
#define SDS_TYPE_16 2
#define SDS_TYPE_32 3
#define SDS_TYPE_64 4



class SDS {
private:

	static size_t sdsReqTypeSize(unsigned char type);
	static unsigned char sdsReqSizeType(size_t string_size);

	size_t len;
	size_t alloc;
	unsigned char flags;
	char* buf;

public:
	char* getBuf()const;

	SDS(const char* init);

	SDS(size_t initlen);
	~SDS();

	size_t length() const;

	size_t avail() const;
	SDS& append(const char* c);
	SDS& append(const SDS& sds);

	SDS& range(int s, int e);

	SDS& clear();

	SDS& operator=(const char* s);

	SDS& operator=(const SDS& sds);



	bool operator==(const char* s) const;

	bool operator==(const SDS& sds) const;


	bool operator!=(const char* s) const;

	bool operator!=(const SDS& sds) const;


};




