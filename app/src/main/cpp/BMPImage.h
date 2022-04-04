#pragma once
#include <cstdint>
#include <array>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>

#define LOG(x) std::cout << x 

struct Pixel {
	uint8_t m_Blue;
	uint8_t m_Green;
	uint8_t m_Red;

	Pixel(uint8_t blue, uint8_t green, uint8_t red)
		:
		m_Green(green),
		m_Blue(blue),
		m_Red(red)
	{}

};

class BMPImage
{
	uint16_t m_Width;
	uint16_t m_Height;

	std::array<uint8_t, 14> fileHeader = {'B','M', 0,0,0,0, 0,0, 0,0, 26,0,0,0};
	std::array<uint8_t, 12> dibHeader = {12,0,0,0, 0,0, 0,0, 1,0, 24,0};
	std::vector<Pixel> m_PixelArray;


public:
	BMPImage(const std::vector<Pixel>& pixelArray, uint16_t width, uint16_t height)
	{
		setDimensions(width, height);
		
		setData(pixelArray);
	}

	BMPImage(){}

	void save(const std::string& fileName) {
		std::ofstream file(fileName, std::fstream::binary);

		auto formattedData = this->format();
		file.write((char*)formattedData.data(), formattedData.size());

		file.close();
	}

	std::vector<uint8_t> format(){
		std::vector<uint8_t> buffer(fileHeader.size()+dibHeader.size()+m_PixelArray.size()*sizeof(Pixel));
		uint8_t* buffAdr = buffer.data();
		memcpy(buffAdr,fileHeader.data(),fileHeader.size());
		memcpy(buffAdr + fileHeader.size(),dibHeader.data(),dibHeader.size());
		memcpy(buffAdr + fileHeader.size() + dibHeader.size(),m_PixelArray.data(),m_PixelArray.size()*sizeof(Pixel));

		return buffer;
	}

	void setData(const std::vector<Pixel>& pixelArray) {
		m_PixelArray = pixelArray;

		uint32_t fileSize = fileHeader.size() + dibHeader.size() + m_PixelArray.size() * sizeof(Pixel);

		*(uint32_t*)(&fileHeader[2]) = fileSize;
	}


	void setDimensions(uint16_t width, uint16_t height) {
		m_Width = width;
		m_Height = height;

		*(uint16_t*)(&dibHeader[4]) = width;
		*(uint16_t*)(&dibHeader[6]) = height;
	}
};

