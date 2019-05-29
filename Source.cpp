#include <iostream>
#include <string>
#include "Header.h"

using namespace std;
int main(int agvc, const char *argv[]) {
	picture bmp;
	const char *file = argv[1];
	const char *result = argv[2];
	const char *buff = argv[3];
	int qwe = stoi(buff);
	if (qwe > 0) {
		bmp.readPicture(file);
		bmp.writePicture(result, qwe);
	}
	else {
		bmp.readPicture(file);
		bmp.writePicture1(result);
		bmp.readPicture(result);
		int p = -qwe;
		bmp.writePicture(result, p);
	}
	return 0;
}