#include "Header.h"
#include <iostream>
#include <fstream>
#include <string>

void picture::readPicture(const char *FileName) {

	FILE *image = fopen(FileName, "rb");
	if (!image) {
		cout << "File could not be open\n";
		exit(1);
	}

	fread(&header.id, sizeof(header.id), 1, image);
	fread(&header.filesize, sizeof(header.filesize), 1, image);
	fread(&header.reserved, sizeof(header.reserved), 1, image);
	fread(&header.headersize, sizeof(header.headersize), 1, image);
	fread(&header.infoSize, sizeof(header.infoSize), 1, image);
	fread(&header.width, sizeof(header.width), 1, image);
	fread(&header.depth, sizeof(header.depth), 1, image);
	fread(&header.biPlanes, sizeof(header.biPlanes), 1, image);
	fread(&header.bits, sizeof(header.bits), 1, image);
	fread(&header.biCompression, sizeof(header.biCompression), 1, image);
	fread(&header.biSizeImage, sizeof(header.biSizeImage), 1, image);
	fread(&header.biXPelsPerMeter, sizeof(header.biXPelsPerMeter), 1, image);
	fread(&header.biYPelsPerMeter, sizeof(header.biYPelsPerMeter), 1, image);
	fread(&header.biClrUsed, sizeof(header.biClrUsed), 1, image);
	fread(&header.biClrImportant, sizeof(header.biClrImportant), 1, image);

	int check = (header.width * 3) % 4;
	int stride = 0;
	if (check) {
		stride = 4 - check;
	}

	array = new RGBQUAD*[header.depth];
	for (int i = 0; i < header.depth; i++) {
		array[i] = new RGBQUAD[header.width];
	}

	for (int i = 0; i < header.depth; i++) {
		for (int j = 0; j < header.width; j++) {
			fread(&array[i][j].blueComponent, 1, 1, image);
			fread(&array[i][j].greenComponent, 1, 1, image);
			fread(&array[i][j].redComponent, 1, 1, image);
		}
		if (stride != 0) {
			RGBQUAD empty;
			fread(&empty, 1, stride, image);
		}
	}
	fclose(image);

}

void picture::writePicture(const char *SecondFile, int factor)
{
	FILE *image2 = fopen(SecondFile, "wb");

	header.depth = header.depth * factor;
	header.width = header.width * factor;

	int check = (header.width * 3) % 4;
	int stride = 0;
	if (check) {
		stride = 4 - check;
	}

	header.biSizeImage = header.depth*header.width * 3 + stride * header.depth;
	header.filesize = header.biSizeImage + sizeof(BMPHEAD);

	RGBQUAD **ARRAY = new RGBQUAD*[header.depth];
	for (int i = 0; i < header.depth; i++) {
		ARRAY[i] = new RGBQUAD[header.width];
	}

	for (int i = 0, q = 0; i < header.depth, q < header.depth / factor; i += factor, q++) {
		for (int j = 0, w = 0; j < header.width, w < header.width / factor; j += factor, w++) {
			for (int k = 0; k < factor; k++) {
				for (int f = 0; f < factor; f++) {
					ARRAY[i + k][j + f] = array[q][w];
				}
			}
		}
	}

	fwrite(&header.id, sizeof(header.id), 1, image2);
	fwrite(&header.filesize, sizeof(header.filesize), 1, image2);
	fwrite(&header.reserved, sizeof(header.reserved), 1, image2);
	fwrite(&header.headersize, sizeof(header.headersize), 1, image2);
	fwrite(&header.infoSize, sizeof(header.infoSize), 1, image2);
	fwrite(&header.width, sizeof(header.width), 1, image2);
	fwrite(&header.depth, sizeof(header.depth), 1, image2);
	fwrite(&header.biPlanes, sizeof(header.biPlanes), 1, image2);
	fwrite(&header.bits, sizeof(header.bits), 1, image2);
	fwrite(&header.biCompression, sizeof(header.biCompression), 1, image2);
	fwrite(&header.biSizeImage, sizeof(header.biSizeImage), 1, image2);
	fwrite(&header.biXPelsPerMeter, sizeof(header.biXPelsPerMeter), 1, image2);
	fwrite(&header.biYPelsPerMeter, sizeof(header.biYPelsPerMeter), 1, image2);
	fwrite(&header.biClrUsed, sizeof(header.biClrUsed), 1, image2);
	fwrite(&header.biClrImportant, sizeof(header.biClrImportant), 1, image2);

	for (int i = 0; i < header.depth; i++) {
		for (int j = 0; j < header.width; j++) {
			fwrite(&ARRAY[i][j].blueComponent, 1, 1, image2);
			fwrite(&ARRAY[i][j].greenComponent, 1, 1, image2);
			fwrite(&ARRAY[i][j].redComponent, 1, 1, image2);
		}
		int8_t empty = 0x00;
		if (stride != 0) {
			fwrite(&empty, 1, stride, image2);
		}
	}

	fclose(image2);
}

void picture::writePicture1(const char *SecondFile)
{
	FILE *image2 = fopen(SecondFile, "wb");


	int check = (header.width * 3) % 4;
	int stride = 0;
	if (check) {
		stride = 4 - check;
	}

	header.biSizeImage = header.depth*header.width * 3 + stride * header.depth;
	header.filesize = header.biSizeImage + sizeof(BMPHEAD);

	RGBQUAD **ARRAY = new RGBQUAD*[header.depth];
	for (int i = 0; i < header.depth; i++) {
		ARRAY[i] = new RGBQUAD[header.width];
	}


	for (int i = 0, k = header.depth; i < header.depth, k>0; i++, k--) {
		for (int j = 0, m = header.width; j < header.width, m>0; j++, m--) {
			ARRAY[i][j] = array[k - 1][m - 1];
		}
	}


	fwrite(&header.id, sizeof(header.id), 1, image2);
	fwrite(&header.filesize, sizeof(header.filesize), 1, image2);
	fwrite(&header.reserved, sizeof(header.reserved), 1, image2);
	fwrite(&header.headersize, sizeof(header.headersize), 1, image2);
	fwrite(&header.infoSize, sizeof(header.infoSize), 1, image2);
	fwrite(&header.width, sizeof(header.width), 1, image2);
	fwrite(&header.depth, sizeof(header.depth), 1, image2);
	fwrite(&header.biPlanes, sizeof(header.biPlanes), 1, image2);
	fwrite(&header.bits, sizeof(header.bits), 1, image2);
	fwrite(&header.biCompression, sizeof(header.biCompression), 1, image2);
	fwrite(&header.biSizeImage, sizeof(header.biSizeImage), 1, image2);
	fwrite(&header.biXPelsPerMeter, sizeof(header.biXPelsPerMeter), 1, image2);
	fwrite(&header.biYPelsPerMeter, sizeof(header.biYPelsPerMeter), 1, image2);
	fwrite(&header.biClrUsed, sizeof(header.biClrUsed), 1, image2);
	fwrite(&header.biClrImportant, sizeof(header.biClrImportant), 1, image2);

	for (int i = 0; i < header.depth; i++) {
		for (int j = 0; j < header.width; j++) {
			fwrite(&ARRAY[i][j].blueComponent, 1, 1, image2);
			fwrite(&ARRAY[i][j].greenComponent, 1, 1, image2);
			fwrite(&ARRAY[i][j].redComponent, 1, 1, image2);
		}
		int8_t empty = 0x00;
		if (stride != 0) {
			fwrite(&empty, 1, stride, image2);
		}
	}

	fclose(image2);
}