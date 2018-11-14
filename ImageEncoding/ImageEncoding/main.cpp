

#include <iostream>
#include "ImageEncoding.h"
using namespace std;

int main() {

	ImageEncoding image("C:\\Users\\kryj9\\Documents\\카카오톡 받은 파일\\Images\\RAW\\VEGAS.RAW");
	image.OnCompression("C:\\Users\\kryj9\\Documents\\카카오톡 받은 파일\\Images\\RAW\\VEGAS.JPG");
	return 0;
}
