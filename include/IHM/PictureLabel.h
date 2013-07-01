#ifndef __PICTURELABEL_H__
#define __PICTURELABEL_H__

#include <QtGui>
#include <QLabel>

class PictureLabel : public QLabel
{
        Q_OBJECT
    public :
        PictureLabel(QWidget *parent, QPixmap image);
        ~PictureLabel();
        void addClickableArea(int left, int top, int width, int height, int index);
    signals :
        void areaClicked(int);
    protected :
        void mousePressEvent ( QMouseEvent* e );

    private :
        int scaleX(int x);
        int scaleY(int y);

        int imageWidth_;
        int imageHeight_;
        QList<QRect> clickableArea_;
        QList<int> clickableAreaIndices_;

};
#endif /* __PICTURELABEL_H__ */
