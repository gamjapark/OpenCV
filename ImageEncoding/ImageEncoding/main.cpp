

#include <iostream>
#include "ImageEncoding.h"
using namespace std;

int main() {

	ImageEncoding image("C:\\Users\\kryj9\\Desktop\\AVE256.jpg");
	image.OnCompression("C:\\Users\\kryj9\\Desktop\\AVE256_new.jpg");
	return 0;
}
