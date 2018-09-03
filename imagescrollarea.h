#ifndef _IMAGESCROLLAREA_H_
#define _IMAGESCROLLAREA_H_

#include <qscrollarea.h>


class ImgScrollArea : public QScrollArea
{
	Q_OBJECT
public:
	ImgScrollArea(){ setAlignment(Qt::AlignCenter); };
	~ImgScrollArea(){};
};

#endif // _IMAGESCROLLAREA_H_

