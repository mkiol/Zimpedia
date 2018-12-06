// Based on: https://github.com/RileyGB/BlackBerry10-Samples

#ifndef WEBIMAGEVIEW_H_
#define WEBIMAGEVIEW_H_

#include <bb/cascades/ImageView>
#include <QNetworkAccessManager>
#include <QNetworkDiskCache>
#include <QUrl>
#include <bb/ImageData>
#include <QImage>
#include <QString>

using namespace bb::cascades;

class WebImageView: public bb::cascades::ImageView {
	Q_OBJECT
	Q_PROPERTY (QUrl url READ url WRITE setUrl NOTIFY urlChanged)
	Q_PROPERTY (float loading READ loading NOTIFY loadingChanged)
	Q_PROPERTY (bool isLoaded READ isLoaded NOTIFY isLoadedChanged)
	Q_PROPERTY (int width READ getWidth NOTIFY widthChanged)

public:
	WebImageView();
	const QUrl& url() const;
	double loading() const;
	bool isLoaded() const;
	int getWidth() const;
	bool getDoSizeCheck();

public Q_SLOTS:
	void setUrl(const QUrl& url);
    void clearCache();

private Q_SLOTS:
	void imageLoaded();
	void dowloadProgressed(qint64,qint64);
	void metaDataChanged();

signals:
	void urlChanged();
	void loadingChanged();
	void isLoadedChanged();
	void widthChanged();

private:
	static QNetworkAccessManager * mNetManager;
	static QNetworkDiskCache * mNetworkDiskCache;

	QUrl mUrl;
	float mLoading;
	bool mIsLoaded;

	bool isARedirectedUrl(QNetworkReply *reply);
	void setURLToRedirectedUrl(QNetworkReply *reply);

    static bb::ImageData fromQImage(const QImage &qImage);
    static Image makeImageWithBackground(const QString &path);

    int sourceSize;
    int sourceWidth;
    int sourceHeight;
};

#endif /* WEBIMAGEVIEW_H_ */
