#ifndef WRAPPER_H
#define WRAPPER_H

#include <QString>
#include <QVector>
#include <QtXml/QDomDocument>
#include <QByteArray>
#include <QRegExp>
#include <QDateTime>

class wrapper
{
public:
    wrapper();
    wrapper(QString *sourceString);
    ~wrapper();

    bool loadFromString(QString *sourceString);
    QVector<QString> getHtmlNodes();

private:
    QDomDocument *dom;
    QByteArray *sourceData;

    struct htmlNode
    {
        QString title;
        QString description;
        QString fulltext;
        QString link;
        QString pubdate;
        QString imageurl;

        void clearContent() {
            this->title.clear();
            this->description.clear();
            this->fulltext.clear();
            this->link.clear();
            this->pubdate.clear();
            this->imageurl.clear();
        }
    };
};

#endif // WRAPPER_H
