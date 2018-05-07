#include <iostream>
#include<fstream>

#include "NN.h"

using namespace std;


double* loadBMP(const char * imagepath){
	// Data read from the header of the BMP file
	unsigned char header[54]; // Each BMP file begins by a 54-bytes header
	unsigned int dataPos;     // Position in the file where the actual data begins
	unsigned int width, height;
	unsigned int imageSize;   // = width*height*3
	// Actual RGB data
	

	// Open the file
	FILE * file;
	fopen_s(&file, imagepath, "rb");
	if (!file)        
	{
		printf("Image could not be opened\n"); 
		printf(imagepath); 
		return 0; 
	}

	if (fread(header, 1, 54, file) != 54){ // If not 54 bytes read : problem
		printf("Not a correct BMP file\n");
		return false;
	}

	if (header[0] != 'B' || header[1] != 'M'){
		printf("Not a correct BMP file\n");
		return 0;
	}

	// Read ints from the byte array
	dataPos = *(int*)&(header[0x0A]);
	imageSize = *(int*)&(header[0x22]);
	width = *(int*)&(header[0x12]);
	height = *(int*)&(header[0x16]);

	// Some BMP files are misformatted, guess missing information
	if (imageSize == 0)    imageSize = width*height * 3; // 3 : one byte for each Red, Green and Blue component
	if (dataPos == 0)      dataPos = 54; // The BMP header is done that way

	// Create a buffer
	unsigned char *data = new unsigned char[imageSize];

	// Read the actual data from the file into the buffer
	fread(data, 1, imageSize, file);

	double *ret=new double[imageSize];
	for(int i=0;i<imageSize;i++)
		ret[i]=data[i];

	//Everything is in memory now, the file can be closed
	fclose(file);

	return ret;
}

int main(){

	const int NO_OF_LAYERS=3;
	const int IMAGE_SIZE=3072;
	int arr[NO_OF_LAYERS]={IMAGE_SIZE,IMAGE_SIZE,1};

	NeuralNetwork obj(NO_OF_LAYERS,arr,0.4,1);

	const int NO_OF_IMAGES=11;
	double *IMAGES[NO_OF_IMAGES];

	IMAGES[0]=loadBMP("A1.bmp");
	IMAGES[1]=loadBMP("B1.bmp");
	IMAGES[2]=loadBMP("C1.bmp");
	IMAGES[3]=loadBMP("D1.bmp");

	IMAGES[4]=loadBMP("Leaf 1.bmp");
	IMAGES[5]=loadBMP("Leaf 2.bmp");
	IMAGES[6]=loadBMP("Leaf 3.bmp");
	IMAGES[7]=loadBMP("Leaf 4.bmp");

	IMAGES[8]=loadBMP("Pen 1.bmp");
	IMAGES[9]=loadBMP("Pen 2.bmp");
	IMAGES[10]=loadBMP("Pen 3.bmp");

	double output[NO_OF_IMAGES]={0.0,0.0,0.0,0.0,1.0,1.0,1.0,1.0,0.0,0.0,0.0};
		
	double input[NO_OF_IMAGES][IMAGE_SIZE];

	for(int i=0;i<NO_OF_IMAGES;i++){
		for(int j=0;j<IMAGE_SIZE;j++)
			input[i][j]=IMAGES[i][j];
	}
	
	obj.Train(input,output,NO_OF_IMAGES,100);

	double *out;
		
	const int NO_OF_TEST_IMAGES=12;
	double *TEST_IMAGES[NO_OF_TEST_IMAGES];

	TEST_IMAGES[0]=loadBMP("T11.bmp");
	TEST_IMAGES[1]=loadBMP("T2.bmp");
	TEST_IMAGES[2]=loadBMP("T3.bmp");
	TEST_IMAGES[3]=loadBMP("T4.bmp");
	TEST_IMAGES[4]=loadBMP("T5.bmp");
	TEST_IMAGES[5]=loadBMP("T6.bmp");
	TEST_IMAGES[6]=loadBMP("T7.bmp");
	TEST_IMAGES[7]=loadBMP("T10.bmp");
	TEST_IMAGES[8]=loadBMP("T8.bmp");
	TEST_IMAGES[9]=loadBMP("T9.bmp");
	TEST_IMAGES[10]=loadBMP("T1.bmp");
	TEST_IMAGES[11]=loadBMP("T12.bmp");
	

	for(int i=0;i<NO_OF_TEST_IMAGES;i++){
		out=obj.Test(TEST_IMAGES[i]);
		
		if(*out<-400){
			cout<<"Test #"<<i+1<<": Leaf\n";
		}
		else{
			cout<<"Test #"<<i+1<<": Not a Leaf\n";
		}
	}

	cout<<endl<<endl;
	
	system("pause");
	return 0;
}