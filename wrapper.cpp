#include "wrapper.h"

wrapper::wrapper()
{
    dom = new QDomDocument;
}

wrapper::wrapper(QString *sourceString)
{
    dom = new QDomDocument;
    sourceData = new QByteArray;
    sourceData->append(*sourceString);
    dom->setContent(*sourceData);
}

wrapper::~wrapper()
{
    delete sourceData;
    delete dom;
}

bool wrapper::loadFromString(QString *sourceString)
{
    if(sourceString->isNull())
    {
        return 1;
    }
    else
    {
        sourceData = new QByteArray;
        sourceData->append(*sourceString);
        dom->setContent(*sourceData);
    }
    return 0;
}

//main function to parse XML to html nodes
QVector<QString> wrapper::getHtmlNodes()
{
    QVector<QString> resultNodes;
    QDomNodeList nodeLst, itemNodes;
    htmlNode htmlN;
    QRegExp imageTag("<img.*>");
    imageTag.setCaseSensitivity(Qt::CaseInsensitive);
    QDateTime pubDate;

    nodeLst = dom->elementsByTagName("item");
    for(int i = 0; i < nodeLst.length(); i++)
    {
        //resultNodes.append(nodeLst->at(i).toElement().text());
        htmlN.clearContent();
        itemNodes = nodeLst.at(i).childNodes();
        for(int j = 0; j < itemNodes.length(); j++) {
            if(itemNodes.at(j).nodeName() == "title") {
                htmlN.title =  itemNodes.at(j).toElement().text().remove(imageTag);
            }
            if(itemNodes.at(j).nodeName() == "description") {
                htmlN.description =  itemNodes.at(j).toElement().text().remove(imageTag);
            }
            else
            if(itemNodes.at(j).nodeName() == "link") {
                htmlN.link = itemNodes.at(j).toElement().text().remove(imageTag);
            }
            else
            if(itemNodes.at(j).nodeName() == "pubDate") {
                pubDate = QDateTime::fromString(itemNodes.at(j).toElement().text().remove(" GMT", Qt::CaseInsensitive), "ddd, dd MMM yyyy hh:mm:ss");
                htmlN.pubdate = !pubDate.isNull() ? pubDate.toString("dd.MM.yyyy hh:mm:ss") : itemNodes.at(j).toElement().text();
            }
            else {
                continue;
            }
        }
        resultNodes.append("<div style=\"width: 99%; margin: 0px; margin-bottom: 5px; padding: 2px;\">");
        resultNodes.append("<p style=\"font-size: 22px; font-weight: bold; padding: 0px; margin: 0px; margin-bottom: 0px;\">" + htmlN.title + "</p>");
        resultNodes.append("<p style=\"font-size: 16px; font-weight: normal; padding: 0px; margin:0px; margin-top: 0px;\">[" + htmlN.pubdate + "]</p>");
        resultNodes.append("<br /><div style=\"font-size: 17px; font-weight: normal; margin:0px; margin-left: 5px; padding: 0px;\">" + htmlN.description + "</div>");
        //resultNodes.append("<a href=\"" + htmlN.link + "\"><p style=\"font-size: 13px; color: #cccccc; font-weight: normal; padding: 0px; margin:0px; margin-top: 0px;\">Читать полностью -></p></a>");
        resultNodes.append("</div>");
        if(i != nodeLst.length() - 1) {
            resultNodes.append("<hr />");
        }
    }

    return resultNodes;
}
