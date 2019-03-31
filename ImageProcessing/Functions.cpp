#include "Functions.h"
#include <windows.h>

using namespace System;
using namespace System::Drawing;
using namespace System::Windows::Forms;

/*BYTE* LoadBMP(int* width, int* height, long* size, LPCTSTR bmpfile)
{
	BITMAPFILEHEADER bmpheader;
	BITMAPINFOHEADER bmpinfo;
	DWORD bytesread;
	HANDLE file = CreateFile(bmpfile, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_FLAG_SEQUENTIAL_SCAN, NULL);

	if (NULL == file)
		return NULL; 

	if (ReadFile(file, &bmpheader, sizeof(BITMAPFILEHEADER), &bytesread, NULL) == false) 
	{
		CloseHandle(file);
		return NULL;
	}

	if (ReadFile(file, &bmpinfo, sizeof(BITMAPINFOHEADER), &bytesread, NULL) == false) 
	{
		CloseHandle(file);
		return NULL;
	}

	if (bmpheader.bfType != 'MB') 
	{
		CloseHandle(file);
		return NULL;
	}

	*width = bmpinfo.biWidth;
	*height = abs(bmpinfo.biHeight);

	if (bmpinfo.biCompression != BI_RGB) 
	{
		CloseHandle(file);
		return NULL;
	}

	if (bmpinfo.biBitCount != 24)
	{
		CloseHandle(file);
		return NULL;
	}

	*size = bmpheader.bfSize - bmpheader.bfOffBits;
	BYTE* Buffer = new BYTE[*size];

	SetFilePointer(file, bmpheader.bfOffBits, NULL, FILE_BEGIN);

	if (ReadFile(file, Buffer, *size, &bytesread, NULL) == false) 
	{
		delete[] Buffer;
		CloseHandle(file);
		return NULL;
	}

	CloseHandle(file);
	// edit by smhgnl
	BYTE* newbuf = new BYTE[*width * *height * 3];

	for (int i = 0; i < *height; i++)
	{
		for (int j = 0; j < *width * 3; j++)
			newbuf[i * *width * 3 + j] = Buffer[(*height - i - 1) * *width * 3 + j];
	}

	return newbuf;
}//LOADPMB*/
/*BYTE* BMPToIntensity(BYTE* Buffer, int width, int height)
{
if ((NULL == Buffer) || (width == 0) || (height == 0))
return NULL;

int padding = 0;
int scanlinebytes = width * 3;

while ((scanlinebytes + padding) % 4 != 0)
padding++;

int psw = scanlinebytes + padding;

BYTE* newbuf = new BYTE[width*height];

long bufpos = 0;
long newpos = 0;

for (int row = 0; row < height; row++)
{
for (int column = 0; column < width; column++)
{
newpos = row * width + column;
// edit by smhgnl
bufpos = row * psw + column * 3;
newbuf[newpos] = BYTE(0.11*Buffer[bufpos + 2] + 0.59*Buffer[bufpos + 1] + 0.3*Buffer[bufpos]);
}
}

return newbuf;
}//ConvetBMPToIntensity*/

BYTE* LoadBMP(int* width, int* height, long* size, LPCTSTR bmpfile)
{
	// declare bitmap structures
	BITMAPFILEHEADER bmpheader;
	BITMAPINFOHEADER bmpinfo;
	// value to be used in ReadFile funcs
	DWORD bytesread;
	// open file to read from
	HANDLE file = CreateFile(bmpfile, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_FLAG_SEQUENTIAL_SCAN, NULL);
	if (NULL == file)
		return NULL; // coudn't open file

					 // read file header
	if (ReadFile(file, &bmpheader, sizeof(BITMAPFILEHEADER), &bytesread, NULL) == false) {
		CloseHandle(file);
		return NULL;
	}
	//read bitmap info
	if (ReadFile(file, &bmpinfo, sizeof(BITMAPINFOHEADER), &bytesread, NULL) == false) {
		CloseHandle(file);
		return NULL;
	}
	// check if file is actually a bmp
	if (bmpheader.bfType != 'MB') {
		CloseHandle(file);
		return NULL;
	}
	// get image measurements
	*width = bmpinfo.biWidth;
	*height = abs(bmpinfo.biHeight);

	// check if bmp is uncompressed
	if (bmpinfo.biCompression != BI_RGB) {
		CloseHandle(file);
		return NULL;
	}
	// check if we have 24 bit bmp
	if (bmpinfo.biBitCount != 24) {
		CloseHandle(file);
		return NULL;
	}

	// create buffer to hold the data
	*size = bmpheader.bfSize - bmpheader.bfOffBits;
	BYTE* Buffer = new BYTE[*size];
	// move file pointer to start of bitmap data
	SetFilePointer(file, bmpheader.bfOffBits, NULL, FILE_BEGIN);
	// read bmp data
	if (ReadFile(file, Buffer, *size, &bytesread, NULL) == false) {
		delete[] Buffer;
		CloseHandle(file);
		return NULL;
	}
	// everything successful here: close file and return buffer
	CloseHandle(file);

	return Buffer;
}//LOADPMB

BYTE* BMPToIntensity(BYTE* Buffer, int width, int height)
{
	// first make sure the parameters are valid
	if ((NULL == Buffer) || (width == 0) || (height == 0))
		return NULL;

	// find the number of padding bytes

	int padding = 0;
	int scanlinebytes = width * 3;
	while ((scanlinebytes + padding) % 4 != 0)     // DWORD = 4 bytes
		padding++;
	// get the padded scanline width
	int psw = scanlinebytes + padding;

	// create new buffer
	BYTE* newbuf = new BYTE[width*height];

	// now we loop trough all bytes of the original buffer, 
	// swap the R and B bytes and the scanlines
	long bufpos = 0;
	long newpos = 0;
	for (int row = 0; row < height; row++)
		for (int column = 0; column < width; column++) {
			newpos = row * width + column;
			bufpos = (height - row - 1) * psw + column * 3;
			newbuf[newpos] = BYTE(0.11*Buffer[bufpos + 2] + 0.59*Buffer[bufpos + 1] + 0.3*Buffer[bufpos]);
		}

	return newbuf;
}//BMPToIntensity
// Intensity Functions
void SurfaceSetPixel(BYTE* Buffer, System::Windows::Forms::PictureBox^ picturebox, int width, int height)
{
	Bitmap^ surface = gcnew Bitmap(width, height);
	picturebox->Image = surface;

	Color color;
	int temp;

	for (unsigned int row = 0; row < height; row++) {
		for (unsigned int column = 0; column < width; column++)
		{
			temp = row * width + column;
			color = Color::FromArgb(Buffer[temp], Buffer[temp], Buffer[temp]);
			surface->SetPixel(column, row, color);
		}
	}
}//SurfaceSetPixel

BYTE* CropImage(BYTE* Buffer, int width, int height, int x_first, int y_first, int x_last, int y_last)
{
	/*int CWidth = x_last - x_first;
	int CHeight = y_last - y_first;
	int CSize = CWidth * CHeight;

	BYTE* Crop = new BYTE[CSize];

	for (int i = 0, j = y_first * width + x_first; i < CSize; i++)
	{
		Crop[i] = Buffer[j];

		if ((i + 1) % CWidth == 0)
			j += width- (CWidth - 1);
		else
			j++;
	}

	return Crop;*/
	int CWidth = x_last - x_first;
	int CHeight = y_last - y_first;
	int CSize = CWidth * CHeight;

	BYTE* Crop = new BYTE[CSize];

	int k = (y_first * width + x_first);

	for (int i = 0; i < CHeight; i++)
	{
		for (int j = 0; j < CWidth; j++)
			Crop[i * CWidth + j] = Buffer[k + j];

		k += width;
	}

	return Crop;
}//CropImage

BYTE* ZoomImage(BYTE* Buffer, int width, int height)
{
	int CropWidth = width;
	int CropHeight = height;
	int ZBWidth = CropWidth * 2 + 1;
	int ZBHeight = CropHeight * 2 + 1;
	int ZIWidth = CropWidth * 2 - 1;
	int ZIHeight = CropHeight * 2 - 1;
	int CropSize = CropWidth * CropHeight;
	int ZBSize = ZBWidth * ZBHeight;
	int ZISize = ZIWidth * ZIHeight;
	float ZoomMask[9] = { 0.25, 0.5, 0.25, 0.5, 1.0, 0.5, 0.25, 0.5, 0.25 };

	BYTE* ZoomBuffer = new BYTE[ZBSize];
	BYTE* ZoomImage = new BYTE[ZISize];

	for (int i = 0; i < ZBSize; i++)
		ZoomBuffer[i] = 0;

	for (int i = 0; i < ZISize; i++)
		ZoomImage[i] = 0;

	for (int i = 0, j = ZBWidth + 1; i < CropSize; i++) 
	{
		ZoomBuffer[j] = Buffer[i];
		if ((i + 1) % CropWidth == 0)
			j += (ZBWidth * 2) - ((CropWidth - 1) * 2);
		else
			j += 2;
	}

	for (int i = 0, j = 0, k = 1; i < ZISize; i++, k++)
	{
		ZoomImage[i] =
			ZoomBuffer[j + i] * ZoomMask[0] +
			ZoomBuffer[j + i + 1] * ZoomMask[1] +
			ZoomBuffer[j + i + 2] * ZoomMask[2] +
			ZoomBuffer[j + i + ZBWidth] * ZoomMask[3] +
			ZoomBuffer[j + i + ZBWidth + 1] * ZoomMask[4] +
			ZoomBuffer[j + i + ZBWidth + 2] * ZoomMask[5] +
			ZoomBuffer[j + i + (ZBWidth * 2)] * ZoomMask[6] +
			ZoomBuffer[j + i + (ZBWidth * 2) + 1] * ZoomMask[7] +
			ZoomBuffer[j + i + (ZBWidth * 2) + 2] * ZoomMask[8];

		if (k % ZIWidth == 0)
			j += 2;
	}

	return ZoomImage;
}//ZoomImage

void HistogramDraw(BYTE* Buffer, System::Windows::Forms::DataVisualization::Charting::Chart^ chart, int width, int height)
{
	int Histogram[256];

	for (int i = 0; i < 256; i++)
		Histogram[i] = 0;

	for (int i = 0; i < width * height; i++)
		Histogram[Buffer[i]] += 1;

	chart->Series->Clear();
	chart->Series->Add("Value");

	for (int i = 0; i < 256; i++)
		chart->Series["Value"]->Points->AddXY(i + 1, Histogram[i]);
}//Histogram

BYTE* HistogramEqualized(BYTE* Buffer, int width, int height)
{
	int Pixel[256];
	int NewPixel[256];
	int PixelSum = width * height;

	BYTE* Equalized = new BYTE[PixelSum];

	for (int i = 0; i < 256; i++) 
	{
		Pixel[i] = 0;
		NewPixel[i] = 0;
	}

	for (int i = 0; i < PixelSum; i++)
		Pixel[Buffer[i]] += 1;

	for (int i = 1; i < 256; i++)
		Pixel[i] += Pixel[i - 1];

	for (int i = 0; i < 256; i++)
		NewPixel[i] = (int)(Pixel[i] * 256) / PixelSum;

	for (int i = 0; i < PixelSum; i++)
		Equalized[i] = NewPixel[Buffer[i]];

	return Equalized;
}//HistogramEqualized

BYTE* Dilation(BYTE* Buffer, int width, int height)
{
	BYTE* Dilation = new BYTE[width * height];
	int Neighbor[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
	int Current = 0;

	for (int i = 0; i < width * height; i++)
		Dilation[i] = 0;

	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			Current = (i * width) + j;

			if (Buffer[Current] == 255)
			{
				Neighbor[0] = (i * width) + (j - 1);
				Neighbor[1] = ((i - 1) * width) + (j - 1);
				Neighbor[2] = ((i - 1) * width) + j;
				Neighbor[3] = ((i - 1) * width) + (j + 1);
				Neighbor[4] = (i * width) + (j + 1);
				Neighbor[5] = ((i + 1) * width) + (j + 1);
				Neighbor[6] = ((i + 1) * width) + j;
				Neighbor[7] = ((i + 1) * width) + (j - 1);

				if (Neighbor[0] > -1 && Neighbor[0] < width * height)
					Dilation[Neighbor[0]] = 255;
				if (Neighbor[1] > -1 && Neighbor[1] < width * height)
					Dilation[Neighbor[1]] = 255;
				if (Neighbor[2] > -1 && Neighbor[2] < width * height)
					Dilation[Neighbor[2]] = 255;
				if (Neighbor[3] > -1 && Neighbor[3] < width * height)
					Dilation[Neighbor[3]] = 255;
				if (Neighbor[4] > -1 && Neighbor[4] < width * height)
					Dilation[Neighbor[4]] = 255;
				if (Neighbor[5] > -1 && Neighbor[5] < width * height)
					Dilation[Neighbor[5]] = 255;
				if (Neighbor[6] > -1 && Neighbor[6] < width * height)
					Dilation[Neighbor[6]] = 255;
				if (Neighbor[7] > -1 && Neighbor[7] < width * height)
					Dilation[Neighbor[7]] = 255;
			}
		}
	}

	return Dilation;
}//Dilation

BYTE* Erosion(BYTE* Buffer, int width, int height)
{
	BYTE* Erosion = new BYTE[width * height];
	int Neighbor[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
	int Current = 0;
	bool Yep;

	for (int i = 0; i < width * height; i++)
		Erosion[i] = 0;

	for (int i = 1; i < height - 2; i++)
	{
		for (int j = 1; j < width - 2; j++)
		{
			Current = (i * width) + j;

			if (Buffer[Current] == 255)
			{
				Neighbor[0] = Buffer[(i * width) + (j - 1)];
				Neighbor[1] = Buffer[((i - 1) * width) + (j - 1)];
				Neighbor[2] = Buffer[((i - 1) * width) + j];
				Neighbor[3] = Buffer[((i - 1) * width) + (j + 1)];
				Neighbor[4] = Buffer[(i * width) + (j + 1)];
				Neighbor[5] = Buffer[((i + 1) * width) + (j + 1)];
				Neighbor[6] = Buffer[((i + 1) * width) + j];
				Neighbor[7] = Buffer[((i + 1) * width) + (j - 1)];

				Yep = (Neighbor[0] == 255 && Neighbor[1] == 255 && Neighbor[2] == 255 && Neighbor[3] == 255 && Neighbor[4] == 255 && Neighbor[5] == 255 && Neighbor[6] == 255 && Neighbor[7] == 255);

				if (Yep)
					Erosion[Current] = Buffer[Current];
				else
					Erosion[Current] = 0;
			}
		}
	}

	return Erosion;
}//Erosion

BYTE* Boundary(BYTE* Buffer, int width, int height, int level)
{
	BYTE* Boundary = new BYTE[width * height];;
	BYTE* erosion;

	erosion = Erosion(Buffer, width, height);

	for (int i = 0; i < level; i++)
		erosion = Erosion(erosion, width, height);

	for (int i = 0; i < width * height; i++)
	{
		if (Buffer[i] == erosion[i])
			Boundary[i] = 0;
		else
			Boundary[i] = 255;
	}

	return Boundary;
}//Boundary

BYTE* KMeans(BYTE* Buffer, int width, int height, int count, int* loop)
{
	*loop = 0;
	BYTE* KMeans = new BYTE[width * height];
	float* Dot = new float[count];
	float* DotTemp = new float[count];
	int* DotDiv = new int[count];
	float Distance = 999;
	int DotSelect = 0;
	int ExitCount = 0;
	bool Average = true;

	for (int i = 0; i < count; i++) 
	{
		Dot[i] = i * 255 / (count - 1);
		DotTemp[i] = 0;
		DotDiv[i] = 0;
	}

	while (Average && *loop < 1000)
	{
		for (int i = 0; i < width * height; i++)
		{
			for (int j = 0; j < count; j++)
			{
				if (Math::Abs(Dot[j] - Buffer[i]) < Distance)
				{
					DotSelect = j;
					Distance = Math::Abs(Dot[j] - Buffer[i]);
				}
			}

			DotTemp[DotSelect] += Buffer[i];
			DotDiv[DotSelect]++;
			Distance = 999;
			DotSelect = 0;
		}

		for (int i = 0; i < count; i++)
		{
			DotTemp[i] /= DotDiv[i];

			if (Math::Abs(DotTemp[i] - Dot[i]) < 0.01F)
				ExitCount++;
		}

		if (ExitCount == count)
			Average = false;
		else
		{
			for (int i = 0; i < count; i++)
			{
				Dot[i] = DotTemp[i];
			}

			for (int i = 0; i < count; i++)
			{
				DotTemp[i] = 0;
				DotDiv[i] = 0;
			}

			Distance = 999;
			DotSelect = 0;
			ExitCount = 0;
		}

		*loop = *loop + 1;
	}

	for (int i = 0; i < width * height; i++)
	{
		for (int j = 0; j < count; j++)
		{
			if (Math::Abs(Dot[j] - Buffer[i]) < Distance)
			{
				DotSelect = j;
				Distance = Math::Abs(Dot[j] - Buffer[i]);
			}
		}

		KMeans[i] = DotSelect * 255 / (count - 1);
		Distance = 999;
	}

	return KMeans;
}//KMenas

BYTE* Labeling(BYTE* Buffer, int width, int height, int* count)
{
	BYTE* LabelLast = new BYTE[width * height];
	int* Label = new int[width * height];
	int Neighbor[4] = { 0, 0, 0, 0 };
	// Prev - TopLeft - Top - TopRight
	int LabelCount = 0;
	int Current = 0;
	int LabelSelect = 0;

	for (int i = 0; i < width * height; i++)
	{
		LabelLast[i] = 0;
		Label[i] = 0;
	}

	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			Current = i * width + j;

			if (Buffer[Current] != 0)
			{
				Neighbor[0] = (i * width) + (j - 1);
				Neighbor[1] = ((i - 1) * width) + (j - 1);
				Neighbor[2] = ((i - 1) * width) + j;
				Neighbor[3] = ((i - 1) * width) + (j + 1);

				if (Neighbor[0] < 0 || Neighbor[0] < (i * width))
					Neighbor[0] = 0;
				else
					Neighbor[0] = Label[Neighbor[0]];

				if (Neighbor[1] < 0 || Neighbor[0] < ((i - 1) * width))
					Neighbor[1] = 0;
				else
					Neighbor[1] = Label[Neighbor[1]];

				if (Neighbor[2] < 0)
					Neighbor[2] = 0;
				else
					Neighbor[2] = Label[Neighbor[2]];

				if (Neighbor[3] < 0 || Neighbor[3] >= i * width)
					Neighbor[3] = 0;
				else
					Neighbor[3] = Label[Neighbor[3]];

				if (Neighbor[0] == 0 && Neighbor[1] == 0 && Neighbor[2] == 0 && Neighbor[3] == 0)
					Label[Current] = ++LabelCount;
				else
				{
					LabelSelect = 255;

					for (int k = 0; k < 4; k++)
					{
						if (Neighbor[k] < LabelSelect && Neighbor[k] > 0)
							LabelSelect = Neighbor[k];
					}

					Label[Current] = LabelSelect;

					for (int k = 0; k < 4; k++)
					{
						if (Neighbor[k] > LabelSelect)
						{
							LabelCount--;
							for (int l = 0; l < i * width + j; l++)
							{
								if (Label[l] == Neighbor[k])
									Label[l] = LabelSelect;
							}
						}
					}
				}
			}
		}
	}

	*count = LabelCount;

	for (int i = 0; i < width * height; i++)
		LabelLast[i] = int(Label[i] * 255 / LabelCount);

	return LabelLast;
}//Labeling

int Moment(BYTE* Buffer, int width, int height, int i, int j)
{
	int temp = 0;

	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			temp += Math::Pow(x, i) * Math::Pow(y, j) * Buffer[(y * width) + x];
		}
	}

	return temp;
}//Moment

double UMoments(BYTE* Buffer, int width, int height, int p, int q)
{
	int m00 = Moment(Buffer, width, height, 0, 0);
	int m01 = Moment(Buffer, width, height, 0, 1);
	int m10 = Moment(Buffer, width, height, 1, 0);

	double xus = m10 / m00;
	double yus = m01 / m00;

	double temp = 0;

	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			temp += Math::Pow((x - xus), p) * Math::Pow((y - yus), q) * Buffer[(y * width) + x];
		}
	}

	return temp;
}//UMoment

double NMoments(BYTE* Buffer, int width, int height, double u00, int i, int j)
{
	double uij = UMoments(Buffer, width, height, i, j);

	return uij / Math::Pow(u00, 1 + ((i + j) / 2));
}//NMoments

double* IMoments(BYTE* Buffer, int width, int height)
{
	double* IM = new double[7];

	double u00 = UMoments(Buffer, width, height, 0, 0);

	double n11 = NMoments(Buffer, width, height, u00, 1, 1);
	double n20 = NMoments(Buffer, width, height, u00, 2, 0);
	double n02 = NMoments(Buffer, width, height, u00, 0, 2);
	double n12 = NMoments(Buffer, width, height, u00, 1, 2);
	double n21 = NMoments(Buffer, width, height, u00, 2, 1);
	double n03 = NMoments(Buffer, width, height, u00, 0, 3);
	double n30 = NMoments(Buffer, width, height, u00, 3, 0);

	IM[0] = n20 + n02;
	IM[1] = Math::Sqrt(n20 - n02) + (4 * Math::Sqrt(n11));
	IM[2] = Math::Sqrt(n30 - (3 * n12)) + Math::Sqrt((3 * n21) - n03);
	IM[3] = Math::Sqrt(n30 + n12) + Math::Sqrt(n21 + n03);
	IM[4] = (n30 - (3 * n12)) * (n30 + n12) * (Math::Sqrt(n30 + n12) - (3 * Math::Sqrt(n21 + n03))) + ((3 * n21) - n03) * (n21 + n03) * ((3 * Math::Sqrt(n30 + n12)) - Math::Sqrt(n21 + n03));
	IM[5] = (n20 - n02) * (Math::Sqrt(n30 + n12) - Math::Sqrt(n21 + n03)) + (4 * n11) * (n30 + n12) * (n21 + n03);
	IM[6] = ((3 * n21) - n03) * (n30 + n12) * (Math::Sqrt(n30 + n12) - (3 * Math::Sqrt(n21 + n03))) - (n30 - (3 * n12)) * (n21 + n03) * ((3 * Math::Sqrt(n30 + n12)) - Math::Sqrt(n21 + n03));

	return IM;
}//IMoments

float* Derivative(BYTE* Buffer, int width, int height, bool direction)
{
	int HorizontalMask[9] = { 1, 2, 1, 0, 0, 0, -1, -2, -1 };
	int VerticalMask[9] = { -1, 0, 1, -2, 0, 2, -1, 0, 1 };
	int DSize = (width - 2) * (height - 2);
	float* Derivative = new float[DSize];
	int Mask[9];

	if (direction)
	{
		for (int i = 0; i < 9; i++)
			Mask[i] = HorizontalMask[i];
	}
	else
	{
		for (int i = 0; i < 9; i++)
			Mask[i] = VerticalMask[i];
	}

	for (int i = 0, j = 0, k = 1; i < DSize; i++, k++)
	{
		Derivative[i] =
			Math::Abs(Buffer[j + i] * Mask[0] +
				Buffer[j + i + 1] * Mask[1] +
				Buffer[j + i + 2] * Mask[2] +
				Buffer[j + i + width] * Mask[3] +
				Buffer[j + i + width + 1] * Mask[4] +
				Buffer[j + i + width + 2] * Mask[5] +
				Buffer[j + i + (width * 2)] * Mask[6] +
				Buffer[j + i + (width * 2) + 1] * Mask[7] +
				Buffer[j + i + (width * 2) + 2] * Mask[8]);

		if (k % (width - 2) == 0)
			j += 2;
	}

	return Derivative;
}//Derivative

float* Degree(float* BufferH, float* BufferV, int width, int height)
{
	float* ArcTan = new float[width * height];

	for (int i = 0; i < width * height; i++)
	{
		if (BufferH[i] != 0)
			ArcTan[i] = Math::Atan(BufferV[i] / BufferH[i]) * 180 / Math::PI;
		else
			ArcTan[i] = Math::Atan(BufferV[i] / 0.01) * 180 / Math::PI;
	}

	return ArcTan;
}//Degree

float* NonMaxSupression(float* BufferH, float* BufferV, float* ArcTan, int width, int height)
{
	int BWidth = width + 2;
	int BHeight = height + 2;
	int Size = width * height;
	int BSize = BWidth * BHeight;
	int Current = 0;
	int ArCurrent = 0;
	int Neighbor[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
	// Prev - TopLeft - Top - TopRight - Next - DownRight - Down - DownLeft
	float* NonMaxSupressionBuf = new float[BSize];
	float* NonMaxSupression = new float[BSize];
	float* EdgeImage = new float[Size];

	for (int i = 0; i < BSize; i++)
		NonMaxSupressionBuf[i] = -1;

	for (int i = 0; i < BSize; i++)
		NonMaxSupression[i] = -1;

	for (int i = 0; i < Size; i++)
		EdgeImage[i] = BufferH[i] + BufferV[i];

	for (int i = 1, j = 1, k = 0; i * BWidth + j < BSize; j++)
	{
		NonMaxSupressionBuf[i * BWidth + j] = EdgeImage[k];
		k++;

		if (j % width == 0)
		{
			j = 0;
			i++;
		}
	}

	for (int i = 0, k = 0; i < BHeight; i++)
	{
		for (int j = 0, l = 0; j < BWidth; j++)
		{
			Current = i * BWidth + j;
			ArCurrent = k * width + l;

			Neighbor[0] = (i * BWidth) + (j - 1);       //Prev
			Neighbor[1] = ((i - 1) * BWidth) + (j - 1); //TopLeft
			Neighbor[2] = ((i - 1) * BWidth) + j;       //Top
			Neighbor[3] = ((i - 1) * BWidth) + (j + 1); //TopRight
			Neighbor[4] = (i * BWidth) + (j + 1);       //Next
			Neighbor[5] = ((i + 1) * BWidth) + (j + 1); //DownRight
			Neighbor[6] = ((i + 1) * BWidth) + j;       //Down
			Neighbor[7] = ((i + 1) * BWidth) + (j - 1); //DownLeft

			if (((ArcTan[ArCurrent] >= 0 && ArcTan[ArCurrent] < 22.5) || (ArcTan[ArCurrent] > 157.5 && ArcTan[ArCurrent] < 180)) && NonMaxSupressionBuf[Current] != -1)
			{
				if ((NonMaxSupressionBuf[Current] < NonMaxSupressionBuf[Neighbor[0]]) || (NonMaxSupressionBuf[Current] < NonMaxSupressionBuf[Neighbor[4]]))
					NonMaxSupression[Current] = 0;
				else
					NonMaxSupression[Current] = NonMaxSupressionBuf[Current];

				if ((l + 1) % width == 0)
					k++;

				l++;
			}
			else if (ArcTan[ArCurrent] > 22.5 && ArcTan[ArCurrent] < 67.5 && NonMaxSupressionBuf[Current] != -1)
			{
				if ((NonMaxSupressionBuf[Current] < NonMaxSupressionBuf[Neighbor[3]]) || (NonMaxSupressionBuf[Current] < NonMaxSupressionBuf[Neighbor[7]]))
					NonMaxSupression[Current] = 0;
				else
					NonMaxSupression[Current] = NonMaxSupressionBuf[Current];

				if ((l + 1) % width == 0)
					k++;

				l++;
			}
			else if (ArcTan[ArCurrent] > 67.5 && ArcTan[ArCurrent] < 112.5 && NonMaxSupressionBuf[Current] != -1)
			{
				if ((NonMaxSupressionBuf[Current] < NonMaxSupressionBuf[Neighbor[2]]) || (NonMaxSupressionBuf[Current] < NonMaxSupressionBuf[Neighbor[6]]))
					NonMaxSupression[Current] = 0;
				else
					NonMaxSupression[Current] = NonMaxSupressionBuf[Current];

				if ((l + 1) % width == 0)
					k++;

				l++;
			}
			else if (ArcTan[ArCurrent] > 112.5 && ArcTan[ArCurrent] < 157.5 && NonMaxSupressionBuf[Current] != -1)
			{
				if ((NonMaxSupressionBuf[Current] < NonMaxSupressionBuf[Neighbor[1]]) || (NonMaxSupressionBuf[Current] < NonMaxSupressionBuf[Neighbor[5]]))
					NonMaxSupression[Current] = 0;
				else
					NonMaxSupression[Current] = NonMaxSupressionBuf[Current];

				if ((l + 1) % width == 0)
					k++;

				l++;
			}
		}
	}

	return NonMaxSupression;
}//NonMaxSupression

BYTE* HysteresisThreshold(float* Buffer, float* ArcTan, int width, int height)
{
	int BWidth = width + 2;
	int BHeight = height + 2;
	int Size = width * height;
	int BSize = BWidth * BHeight;
	int Current = 0;
	int ArCurrent = 0;
	int Neighbor[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
	// Prev - TopLeft - Top - TopRight - Next - DownRight - Down - DownLeft
	int Min = 9999;
	int Max = 0;

	int* HysteresisThresholdBuf = new int[BSize];
	BYTE* HysteresisThreshold = new BYTE[Size];

	for (int i = 0; i < BSize; i++)
	{
		if (Buffer[i] > Max)
			Max = Buffer[i];

		if (Buffer[i] < Min && Buffer[i] > 0)
			Min = Buffer[i];
	}

	//Min = Min / 100 * 10;
	Max = Max / 100 * 50;

	for (int i = 0; i < BSize; i++)
	{
		if (Buffer[i] == Max)
			HysteresisThresholdBuf[i] = 1;
		else if (Buffer[i] == Min)
			HysteresisThresholdBuf[i] = 0;
		else
			HysteresisThresholdBuf[i] = Buffer[i];
	}
	
	for (int i = 0, k = 0; i < BHeight; i++)
	{
		for (int j = 0, l = 0; j < BWidth; j++)
		{
			Current = i * BWidth + j;
			ArCurrent = k * width + l;

			Neighbor[0] = (i * BWidth) + (j - 1);       //Prev
			Neighbor[1] = ((i - 1) * BWidth) + (j - 1); //TopLeft
			Neighbor[2] = ((i - 1) * BWidth) + j;       //Top
			Neighbor[3] = ((i - 1) * BWidth) + (j + 1); //TopRight
			Neighbor[4] = (i * BWidth) + (j + 1);       //Next
			Neighbor[5] = ((i + 1) * BWidth) + (j + 1); //DownRight
			Neighbor[6] = ((i + 1) * BWidth) + j;       //Down
			Neighbor[7] = ((i + 1) * BWidth) + (j - 1); //DownLeft

			if (((ArcTan[ArCurrent] >= 0 && ArcTan[ArCurrent] < 22.5) || (ArcTan[ArCurrent] > 157.5 && ArcTan[ArCurrent] < 180)) && HysteresisThresholdBuf[Current] > 1)
			{
				if (HysteresisThresholdBuf[Neighbor[2]] == 1 || HysteresisThresholdBuf[Neighbor[6]] == 1)
					HysteresisThresholdBuf[Current] = 1;
				//if (HysteresisThresholdBuf[Neighbor[2]] == 0 || HysteresisThresholdBuf[Neighbor[6]] == 0)
				//	HysteresisThresholdBuf[Current] = 0;

				if ((l + 1) % width == 0)
					k++;

				l++;
			}
			else if (ArcTan[ArCurrent] > 22.5 && ArcTan[ArCurrent] < 67.5 && HysteresisThresholdBuf[Current] > 1)
			{
				if (HysteresisThresholdBuf[Neighbor[1]] == 1 || HysteresisThresholdBuf[Neighbor[5]] == 1)
					HysteresisThresholdBuf[Current] = 1;
				//if (HysteresisThresholdBuf[Neighbor[1]] == 0 || HysteresisThresholdBuf[Neighbor[5]] == 0)
				//	HysteresisThresholdBuf[Current] = 0;

				if ((l + 1) % width == 0)
					k++;

				l++;
			}
			else if (ArcTan[ArCurrent] > 67.5 && ArcTan[ArCurrent] < 112.5 && HysteresisThresholdBuf[Current] > 1)
			{
				if (HysteresisThresholdBuf[Neighbor[0]] == 1 || HysteresisThresholdBuf[Neighbor[4]] == 1)
					HysteresisThresholdBuf[Current] = 1;
				//if (HysteresisThresholdBuf[Neighbor[0]] == 0 || HysteresisThresholdBuf[Neighbor[4]] == 0)
				//	HysteresisThresholdBuf[Current] = 0;

				if ((l + 1) % width == 0)
					k++;

				l++;
			}
			else if (ArcTan[ArCurrent] > 112.5 && ArcTan[ArCurrent] < 157.5 && HysteresisThresholdBuf[Current] > 1)
			{
				if (HysteresisThresholdBuf[Neighbor[3]] == 1 || HysteresisThresholdBuf[Neighbor[7]] == 1)
					HysteresisThresholdBuf[Current] = 1;
				//if (HysteresisThresholdBuf[Neighbor[3]] == 0 || HysteresisThresholdBuf[Neighbor[7]] == 0)
				//	HysteresisThresholdBuf[Current] = 0;

				if ((l + 1) % width == 0)
					k++;

				l++;
			}
		}
	}
	
	for (int i = 0; i < BSize; i++)
	{
		if (HysteresisThresholdBuf[i] > 1)
		{
			if (Math::Abs(HysteresisThresholdBuf[i] - Min) < Math::Abs(HysteresisThresholdBuf[i] - Max))
				HysteresisThresholdBuf[i] = 0;
			else
				HysteresisThresholdBuf[i] = 1;
		}
	}

	for (int i = 0, j = 0; i < BSize; i++)
	{
		if (HysteresisThresholdBuf[i] != -1)
		{
			if (HysteresisThresholdBuf[i] == 0)
				HysteresisThreshold[j] = 0;
			else
				HysteresisThreshold[j] = 255;

			j++;
		}
	}

	return HysteresisThreshold;
}//HysteresisThreshold

// RGB Functions
void SurfaceSetPixelRGB(BYTE* Buffer, System::Windows::Forms::PictureBox^ picturebox, int width, int height)
{
	Bitmap^ surface = gcnew Bitmap(width, height);
	picturebox->Image = surface;

	Color color;
	int temp;

	for (unsigned int row = 0; row < height; row++) 
	{
		for (unsigned int column = 0; column < width; column++)
		{
			temp = (row * 3) * width + (column * 3);
			color = Color::FromArgb(Buffer[temp + 2], Buffer[temp + 1], Buffer[temp]);
			surface->SetPixel(column, row, color);
		}
	}
}//SurfaceSetPixelRGB

BYTE* CropImageRGB(BYTE* Buffer, int width, int height, int x_first, int y_first, int x_last, int y_last)
{
	int CWidth = (x_last - x_first) * 3;
	int CHeight = y_last - y_first;
	int CSize = CWidth * CHeight;

	BYTE* Crop = new BYTE[CSize];

	int k = (y_first * width + x_first) * 3;

	for (int i = 0; i < CHeight; i++)
	{
		for (int j = 0; j < CWidth; j++)
			Crop[i * CWidth + j] = Buffer[k + j];

		k += width * 3;
	}

	return Crop;
}//CropImageRGB
