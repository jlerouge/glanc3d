#include "PictureLabel.h"

PictureLabel::PictureLabel(QWidget *parent, QPixmap image):QLabel(parent)
{
    this->setPixmap(image);
    imageWidth_ = image.width();
    imageHeight_ = image.height();
    setScaledContents(true);
}

PictureLabel::~PictureLabel()
{
    clickableArea_.clear();
    clickableAreaIndices_.clear();
}

void PictureLabel::mousePressEvent(QMouseEvent* e)
{
    if (e->button() == Qt::LeftButton)
    {
        if (!this->clickableArea_.isEmpty())
        {
            for (int i=0;i<this->clickableArea_.size();i++)
            {
                if (scaleX(e->x()) > clickableArea_[i].left() &&
                        scaleX(e->x()) < clickableArea_[i].left() + clickableArea_[i].width() &&
                        scaleY(e->y()) > clickableArea_[i].top() &&
                        scaleY(e->y()) < clickableArea_[i].top() + clickableArea_[i].height())
                {
                    emit areaClicked(clickableAreaIndices_[i]);
                    break;
                }
            }
        }
    }
}

void PictureLabel::addClickableArea(int left, int top, int width, int height, int index)
{
    clickableArea_.append(QRect(left, top, width, height));
    clickableAreaIndices_.append(index);
}

int PictureLabel::scaleX(int x)
{
    int retour=((imageWidth_/size().width())*x);
    return retour;
}

int PictureLabel::scaleY(int y)
{
    int retour=((imageHeight_/size().height())*y);
    return retour;
}
//#include <../../moc_PictureLabel.cpp>
