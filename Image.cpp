#include "Image.h"

unsigned char* Image::getData(){
	return this->data;
}
bool Image::setData(unsigned char*){
	this->data = data;
	return true;
}

int Image::getCols(){
	return this->cols;
}
bool Image::setCols(int cols){
	this->cols = cols;
	return true;
}

int Image::getRows(){
	return this->rows;
}
bool Image::setRows(int rows){
	this->rows = rows;
	return true;
}
