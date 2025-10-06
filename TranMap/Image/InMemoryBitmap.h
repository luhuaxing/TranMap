
#pragma once;
#include "base64.h"

// A wrapper for GDI+ bitmap located in the global heap
class CMemoryBitmap
{
private:	
	// icon stream (must be open for the lifetime of the Bitmap according to MSDN Bitmap constructor)
	IStream* m_stream;

	// underlying data for the image (must be released in destructor)
	HGLOBAL m_data;

	int m_size;
public:
	// for tiles
	int Provider;

	HGLOBAL getData()
	{
		return m_data;
	}
	
	// The actual bitmap	
	Gdiplus::Bitmap* m_bitmap;
	
	int get_Width()
	{
		if (m_bitmap)
			return m_bitmap->GetWidth();
		else
			return 0;
	}

	int get_Height()
	{
		if (m_bitmap)
			return m_bitmap->GetHeight();
		else
			return 0;
	}

	int get_Size()
	{
		return m_size;
	}

	// **************************************************
	//    Constructor()
	// **************************************************
	// Creates an empty instance of CMemoryBitmap class
	CMemoryBitmap::CMemoryBitmap()
	{
		m_bitmap = NULL;
		m_stream = NULL;
		m_data = NULL;
		m_size = 0;
	};
	
	// **************************************************
	//    LoadFromBase64String()
	// **************************************************
	// Loads data from base64 string
	bool LoadFromBase64String(std::string str)
	{
		if (str.size() != 0)
		{
			std::string strNew = base64_decode(str);
			const char* data = strNew.c_str();
			
			return this->LoadFromRawData(data, (int)strNew.size());
		}
		return false;
	}

	// **************************************************
	//    SerializeToBase64String()
	// **************************************************
	std::string SerializeToBase64String()
	{
		if (m_data && m_size > 0)
		{
			void* hMem = ::GlobalLock(m_data);
			if (hMem)
			{
				char* data = new char[m_size];
				memcpy(data, hMem, m_size);
				::GlobalUnlock(hMem);
				
				std::string str = base64_encode(reinterpret_cast<unsigned char*>(data), m_size);
				delete[] data;
				return str;
			}
		}
		return "";
	}

	// **************************************************
	//    LoadFromRawData()
	// **************************************************
	bool LoadFromRawData(const char* data, int size)
	{
		this->Release();
		
		if (size <= 0)
			return false;

		m_data = ::GlobalAlloc(GMEM_MOVEABLE, size);
		if (m_data)
		{
			void* hMem = ::GlobalLock(m_data);
			if (hMem)
			{
				memcpy(hMem, data, size);
				::GlobalUnlock(hMem);

				if (::CreateStreamOnHGlobal(m_data, FALSE, &m_stream) == S_OK)
				{
					m_bitmap = new Gdiplus::Bitmap(m_stream);
					m_size = size;
					if (m_bitmap)
						return true;
				}
			}
		}

		this->Release();
		return false;
	}

	// ************************************************
	//	Release()
	// ************************************************
	void Release()
	{
		if (m_bitmap)
		{
			delete m_bitmap;
			m_bitmap = NULL;
		}

		if (m_stream)
		{
			m_stream->Release();
			m_stream = NULL;
		}

		if (m_data)
		{
			m_data = GlobalFree(m_data);
			m_data = NULL;
		}
		m_size = 0;
	}

	// Destructor. Underlying stream must be closed and data deleted
	virtual ~CMemoryBitmap()
	{
		this->Release();
	}
};
