#include "sds.h"


size_t SDS::sdsReqTypeSize(unsigned char type)
{
	switch (type&7)
	{
	case SDS_TYPE_5:return 5;
	case SDS_TYPE_8:return 8;
	case SDS_TYPE_16:return 16;
	case SDS_TYPE_32:return 32;
	case SDS_TYPE_64:return 64;
	}
	return 0;
}

unsigned char SDS::sdsReqSizeType(size_t string_size)
{
	if (string_size < (1 << (SDS_TYPE_5&7)))
	{
		return SDS_TYPE_5 & 7;
	}
	else if (string_size < (1 << (SDS_TYPE_8&7)))
	{
		return SDS_TYPE_8&7;
	}
	else if (string_size < (1 << (SDS_TYPE_16&7)))
	{
		return SDS_TYPE_16&7;
	}
	else if (string_size < (1 << (SDS_TYPE_32&7)))
	{
		return SDS_TYPE_32&7;
	}
	else if (string_size < (1 << (SDS_TYPE_64&7)))
	{
		return SDS_TYPE_64&7;
	}
}

char* SDS::getBuf() const
{
	return this->buf;
}

SDS::SDS(const char* init)
{
	this->len = strlen(init);
	this->flags = sdsReqSizeType(this->len);
	this->alloc = (1 << (this->flags * 7));
	this->buf = new char[this->alloc];
	memcpy(this->buf, init, this->len);
}

SDS::SDS(size_t initlen)
{
	this->len = initlen;
	this->flags = sdsReqSizeType(this->len);
	this->alloc = (1 << (this->flags * 7));
	this->buf = new char[this->alloc];
}

SDS::~SDS()
{
	delete[] buf;
}

size_t SDS::length() const
{
	return this->len;
}

size_t SDS::avail() const
{
	return this->alloc;
}

SDS& SDS::append(const char* c)
{
	// TODO: 在此处插入 return 语句


	size_t new_size = strlen(c) + this->len;

	size_t old_size = this->len;


	if (new_size > this->alloc) //add capacity
	{
		if ((this->flags&7) == (SDS_TYPE_64))
		{
			// can not add capacity
		}
		this->flags = (this->flags & 7) + 1;
		this->alloc = (1 << sdsReqTypeSize(this->flags));
		char* new_ptr = new char[this->alloc];
		if (new_ptr == nullptr)
		{
			throw std::bad_alloc();
		}

		char* old_ptr = this->buf;

		memcpy(new_ptr, old_ptr, old_size);

		delete[] old_ptr;
		this->buf = new_ptr;
	}

	this->len = new_size;
	strcat(this->buf, c);

	return *this;

}

SDS& SDS::append(const SDS& sds)
{
	return append(sds.getBuf());
}

SDS& SDS::range(size_t s, size_t e)
{
	assert(e > s);

	assert(this->len>=e);

	// I do not think we should frequently alloc in the code
	//we can define another func to shrink

	for (size_t i = s; i < e; i++)
	{
		this->buf[i - s] = this->buf[i];
	}

	for (size_t i = e - s; i < this->len; i++)
	{
		this->buf[i] = '\0';
	}

	this->len = (e - s);
	return *this;


}

SDS& SDS::clear()
{
	// the same as range alloc should not happen too much
	for (size_t i = 0; i < this->len; i++)
	{
		this->buf[i] = '\0';
	}
	this->len = 0;
	return *this;

}

SDS& SDS::operator=(const char* s)
{
	this->clear();
	this->append(s);
	return *this;
}

SDS& SDS::operator=(const SDS& sds)
{
	this->clear();
	this->append(sds);
	return *this;
}

bool SDS::operator==(const char* s) const
{
	if (strlen(s) != this->len)
	{
		return false;
	}

	for (size_t i = 0; i < this->len; i++)
	{
		if (this->buf[i] != s[i])
			return false;
	}
	return true;

}

bool SDS::operator==(const SDS& sds) const
{
	if (sds.length() != this->len)
	{
		return false;
	}
	const char* s = sds.getBuf();

	for (size_t i = 0; i < this->len; i++)
	{
		if (this->buf[i] != s[i])
			return false;
	}
	return true;
}

bool SDS::operator!=(const char* s) const
{
	return !(*this==s);
}

bool SDS::operator!=(const SDS& sds) const
{
	return !(*this == sds);
}
