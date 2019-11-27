#ifndef BITCOIN_QT_DOWNLOADER_H
#define BITCOIN_QT_DOWNLOADER_H

#include <QNetworkAccessManager>

class QNetworkReply;
class QFile;

class Downloader : public QObject
{
    Q_OBJECT
    using BaseClass = QObject;

public:
    explicit Downloader(QObject* parent = nullptr);
    virtual ~Downloader();

    bool get(const QString& targetFolder, const QUrl& url, bool &proxy);
    QString getDataName();


public Q_SLOTS:
    void cancelDownload();

Q_SIGNALS:
    void updateDownloadProgress(qint64 bytesReceived, qint64 bytesTotal);
    void onFinished();


private Q_SLOTS:
    void onReadyRead();
    void onReply(QNetworkReply* reply);

private:
    QNetworkReply* m_currentReply {nullptr};
    QFile* m_file                 {nullptr};
    QNetworkAccessManager m_manager;
    void SetDataName(QString name);
    QString fileName;
};

#endif // BITCOIN_QT_DOWNLOADER_H