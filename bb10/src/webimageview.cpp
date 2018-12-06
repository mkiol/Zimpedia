// Based on: https://github.com/RileyGB/BlackBerry10-Samples

#include "webimageview.h"
#include <QNetworkReply>
#include <QNetworkDiskCache>
#include <QDesktopServices>
#include <QStringList>
#include <QImage>
#include <QPainter>
#include <QColor>
#include <QFile>
#include <bb/cascades/Image>

using namespace bb::cascades;

QNetworkAccessManager * WebImageView::mNetManager = new QNetworkAccessManager();
QNetworkDiskCache * WebImageView::mNetworkDiskCache = new QNetworkDiskCache();

WebImageView::WebImageView() : sourceSize(0)
{
    // Initialize network cache
    mNetworkDiskCache->setCacheDirectory(QDesktopServices::storageLocation(QDesktopServices::CacheLocation));

    // Set cache in manager
    mNetManager->setCache(mNetworkDiskCache);

    // Set defaults
    mLoading = 0;
    mIsLoaded = false;
}

const QUrl& WebImageView::url() const
{
    return mUrl;
}

Image WebImageView::makeImageWithBackground(const QString &path)
{
    QImage image(path);
    QImage newImage(image.size(), QImage::Format_ARGB32_Premultiplied);
    newImage.fill(QColor(Qt::white));
    QPainter painter(&newImage);
    painter.drawImage(image.rect(), image);

    return Image(WebImageView::fromQImage(newImage));
}

bb::ImageData WebImageView::fromQImage(const QImage &qImage)
{
    bb::ImageData imageData(bb::PixelFormat::RGBA_Premultiplied, qImage.width(), qImage.height());

    unsigned char *dstLine = imageData.pixels();
    for (int y = 0; y < imageData.height(); y++) {
        unsigned char * dst = dstLine;
        for (int x = 0; x < imageData.width(); x++) {
            QRgb srcPixel = qImage.pixel(x, y);
            *dst++ = qRed(srcPixel) * qAlpha(srcPixel) / 255;
            *dst++ = qGreen(srcPixel) * qAlpha(srcPixel) / 255;
            *dst++ = qBlue(srcPixel) * qAlpha(srcPixel) / 255;
            *dst++ = qAlpha(srcPixel);
        }
        dstLine += imageData.bytesPerLine();
    }

    return imageData;
}

int WebImageView::getWidth() const
{
    return sourceWidth;
}

void WebImageView::setUrl(const QUrl& url)
{
    if (url == mUrl) {
        return;
    }

    mLoading = 0;
    mIsLoaded = false;

    mUrl = url;
    mLoading = 0;
    mIsLoaded = false;
    emit isLoadedChanged();
    resetImage();

    QString _url = url.toString();

    if (url.isEmpty()) {
        emit urlChanged();
        return;
    }

    // Detecting if url is "asset:///"
    if (_url.startsWith("asset:///")) {
        this->setImageSource(url);
        mIsLoaded = true;
        emit isLoadedChanged();
        emit urlChanged();
        return;
    }

    // Detecting if url is "image://icons/"
    if (_url.startsWith("image://icons/")) {
        QString path = QDesktopServices::storageLocation(QDesktopServices::CacheLocation) +
                "/" + _url.right(_url.length() - 14) + ".png";

        if (QFile::exists(path)) {
            setImage(WebImageView::makeImageWithBackground(path));
            mIsLoaded = true;
        } else {
            mIsLoaded = false;
        }

        //this->setImageSource(QUrl(path));

        emit isLoadedChanged();
        emit urlChanged();
        return;
    }

    // Create request
    QNetworkRequest request;
    request.setAttribute(QNetworkRequest::CacheLoadControlAttribute, QNetworkRequest::PreferCache);
    request.setUrl(url);

    // Create reply
    QNetworkReply * reply = mNetManager->get(request);

    // Connect to signals
    QObject::connect(reply, SIGNAL(finished()), this, SLOT(imageLoaded()));
    QObject::connect(reply, SIGNAL(downloadProgress(qint64, qint64)), this, SLOT(dowloadProgressed(qint64,qint64)));
    QObject::connect(reply, SIGNAL(metaDataChanged()), this, SLOT(metaDataChanged()));

    emit urlChanged();
}

void WebImageView::metaDataChanged()
{
    //QNetworkReply * reply = qobject_cast<QNetworkReply*>(sender());
    //qDebug() << "metaDataChanged:" << reply->url();
}

double WebImageView::loading() const
{
    return mLoading;
}

void WebImageView::imageLoaded() {
    // Get reply
    QNetworkReply * reply = qobject_cast<QNetworkReply*>(sender());

    //qDebug() << "error" << reply->error();
    //qDebug() << "imageLoaded:" << reply->url() << reply->error();

    if (reply->error() == QNetworkReply::NoError) {
        if (isARedirectedUrl(reply)) {
            setURLToRedirectedUrl(reply);
            return;
        } else {
            QByteArray imageData = reply->readAll();
            QImage img = QImage::fromData(imageData);
            int width = img.width();
            if (width != sourceWidth) {
                sourceWidth = width;
                emit widthChanged();
            }
            //qDebug() << "ContentType:" << reply->header(QNetworkRequest::ContentTypeHeader).toString();
            if (reply->header(QNetworkRequest::ContentTypeHeader).toString() == "image/x-icon") {
                // BB does not support ICO image format -> must convert
                setImage(Image(fromQImage(QImage::fromData(imageData))));
            } else {
                setImage(Image(imageData));
            }

            mIsLoaded = true;
        }
    } else {
        mIsLoaded = false;
        mUrl.clear();
    }

    emit isLoadedChanged();

    // Memory management
    reply->deleteLater();
}

bool WebImageView::isARedirectedUrl(QNetworkReply *reply) {
    QUrl redirection = reply->attribute(QNetworkRequest::RedirectionTargetAttribute).toUrl();
    return !redirection.isEmpty();
}

bool WebImageView::isLoaded() const {
    return mIsLoaded;
}

void WebImageView::setURLToRedirectedUrl(QNetworkReply *reply) {
    QUrl redirectionUrl = reply->attribute(QNetworkRequest::RedirectionTargetAttribute).toUrl();
    QUrl baseUrl = reply->url();
    QUrl resolvedUrl = baseUrl.resolved(redirectionUrl);

    setUrl(resolvedUrl.toString());
}

void WebImageView::clearCache() {
    mNetworkDiskCache->clear();
}

void WebImageView::dowloadProgressed(qint64 bytes, qint64 total) {
    mLoading = double(bytes) / double(total);

    emit loadingChanged();
}
