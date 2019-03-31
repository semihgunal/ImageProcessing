#include <windows.h>

BYTE* LoadBMP(int* width, int* height, long* size, LPCTSTR bmpfile);
BYTE* BMPToIntensity(BYTE* Buffer, int width, int height);
// Intensity Functions
void SurfaceSetPixel(BYTE* Buffer, System::Windows::Forms::PictureBox^ picturebox, int width, int height);
BYTE* CropImage(BYTE* Buffer, int width, int height, int x_first, int y_first, int x_last, int y_last);
BYTE* ZoomImage(BYTE* Buffer, int width, int height);
void HistogramDraw(BYTE* Buffer, System::Windows::Forms::DataVisualization::Charting::Chart^ chart, int width, int height);
BYTE* HistogramEqualized(BYTE* Buffer, int width, int height);
BYTE* Dilation(BYTE* Buffer, int width, int heigt);
BYTE* Erosion(BYTE* Buffer, int width, int heigt);
BYTE* Boundary(BYTE* Buffer, int width, int height, int level);
BYTE* KMeans(BYTE* Buffer, int width, int height, int count, int* loop);
BYTE* Labeling(BYTE* Buffer, int width, int height, int* count);
int Moment(BYTE* Buffer, int width, int height, int i, int j);
double UMoments(BYTE* Buffer, int width, int height, int p, int q);
double NMoments(BYTE* Buffer, int width, int height, double u00, int i, int j);
double* IMoments(BYTE* Buffer, int width, int height);
float* Derivative(BYTE* Buffer, int width, int height, bool direction);
float* Degree(float* BufferH, float* BufferV, int width, int height);
float* NonMaxSupression(float* BufferH, float* BufferV, float* ArcTan, int width, int height);
BYTE* HysteresisThreshold(float* Buffer, float* ArcTan, int width, int height);
// RGB Functions
void SurfaceSetPixelRGB(BYTE* Buffer, System::Windows::Forms::PictureBox^ picturebox, int width, int height);
BYTE* CropImageRGB(BYTE* Buffer, int width, int height, int x_first, int y_first, int x_last, int y_last);
