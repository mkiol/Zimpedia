#ifndef ZIMMETADATA_H
#define ZIMMETADATA_H

#include <QString>

struct ZimMetaData {
    enum FieldFlags
    {
        None = 0,
        Size = 1,
        Time = 2,
        Checksum = 4,
        Name = 8,
        Title = 16,
        Creator = 32,
        Date = 64,
        Description = 128,
        Language = 256,
        Favicon = 512,
        Filename = 1024,
        Publisher = 2048,
        Source = 4096,
        Tags = 8192,
        //Version = 16384,
        ArticleCount = 32768
    };

    int fields = None;
    QString path;
    QString filename;
    qint64 size = 0;
    QString time;
    QString checksum;
    QString name;
    QString title;
    QString creator;
    QString date;
    QString description;
    QString language;
    QString favicon;
    QString publisher;
    QString source;
    QString tags;
    //QString version;
    int article_count = 0;

    void clear() {
        fields = None;
        path.clear();
        filename.clear();
        size = 0;
        time.clear();
        checksum.clear();
        name.clear();
        title.clear();
        creator.clear();
        date.clear();
        description.clear();
        language.clear();
        favicon.clear();
        publisher.clear();
        source.clear();
        tags.clear();
        article_count = 0;
    }

    bool isEmpty() {
        return fields == None;
    }
};

#endif // ZIMMETADATA_H
