#include "base.h"

Base::Base(){

}

Base::~Base(){

}

bool Base::isEmpty(){
	return ListOfParam.empty();
}
ParamValue Base::getParamValue(int i){
	if (isEmpty()){
		exit(0);
	}
	return ListOfParam.at(i);
}
bool Base::setParamValue(int i,ParamValue pv){
	ListOfParam.at(i) = pv;
	return true;
}
vector<QImage*> Base::getInputImage(){
	return transObj.inputImage;
}
bool Base::setInputImage(QImage* image){
	transObj.inputImage.push_back(image);
	return true;
}
vector<QImage*> Base::getOutputImage(){
	return transObj.outputImage;
}
bool Base::setOutputImage(QImage* image, bool bClean)
{
	if (bClean)
		clearOutImage();

	transObj.outputImage.push_back(image);
	return true;
}



int Base::getNums(){
	return ListOfParam.size();
}



void Base::clearOutImage()
{
	transObj.outputImage.clear();
}