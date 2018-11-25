#include <iostream>
#include "ImageDecoding.h"
using namespace std;

int main() {

	ImageDecoding image("C:\\Users\\kryj9\\Desktop\\VEGAS_new.jpg");
	image.OnDeCompression("C:\\Users\\kryj9\\Desktop\\VEGAS_new.jpg", "C:\\Users\\kryj9\\Desktop\\memem.raw");
	return 0;
}
