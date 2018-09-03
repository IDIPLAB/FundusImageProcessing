#ifndef Image_H
#define Image_H
class Image
{
private:
	unsigned char* data;
	int cols;
	int rows;

public:
	unsigned char* getData();
	bool setData(unsigned char*);
	int getCols();
	bool setCols(int cols);
	int getRows();
	bool setRows(int rows);
};
#endif