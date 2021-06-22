#ifndef IMAGELABEL_H
#define IMAGELABEL_H

#include <QObject>
#include <QLabel>
//#include <globaldef.hpp>


class ImageLabel : public QLabel
{
public:
    ImageLabel(QWidget *parent = NULL);

    QString imageRemovePath;

    ~ImageLabel();

    bool copyFileToPath(QString sourceDir ,QString toDir, bool coverFileIfExist);
protected:
    virtual void mousePressEvent(QMouseEvent *ev) override;
private:


};

#endif // IMAGELABEL_H
