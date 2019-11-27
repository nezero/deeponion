#include "qt/downloader.h"

#include <QNetworkProxy>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QFile>
#include <QDir>

Downloader::Downloader(QObject* parent) :
    BaseClass(parent)
{
    connect(&m_manager, &QNetworkAccessManager::finished, this, &Downloader::onReply);
}

Downloader::~Downloader(){}


bool Downloader::get(const QString& targetFolder, const QUrl& url, bool& proxy)
{
    if (targetFolder.isEmpty() || url.isEmpty())
    {
        return false;
    }

    m_file = new QFile(targetFolder + QDir::separator() + url.fileName());
    if (!m_file->open(QIODevice::WriteOnly))
    {
        delete m_file;
        m_file = nullptr;
        return false;
    }

    //Set Proxy
    //TODO: Merge with init and version check proxy in clientmodel
    if(proxy)
    {
        QNetworkProxy proxy;
        proxy.setType(QNetworkProxy::Socks5Proxy);
        proxy.setHostName("127.0.0.1");
        proxy.setPort(9081);
        m_manager.setProxy(proxy);
     }

    QNetworkRequest request(url);
    request.setAttribute(QNetworkRequest::FollowRedirectsAttribute, true);
    m_currentReply = m_manager.get(request);
    SetDataName(url.fileName());

    connect(m_currentReply, &QNetworkReply::readyRead, this, &Downloader::onReadyRead);
    connect(m_currentReply, &QNetworkReply::downloadProgress, this, &Downloader::updateDownloadProgress);
    connect(m_currentReply, &QNetworkReply::finished, this, &Downloader::onFinished);
    return true;
}

void Downloader::onReadyRead()
{
    if (m_file)
    {
        m_file->write(m_currentReply->readAll());
    }
}

void Downloader::cancelDownload()
{
    if (m_currentReply)
    {
        m_currentReply->abort();
    }
}

void Downloader::onReply(QNetworkReply* reply)
{
    if (reply->error() == QNetworkReply::NoError)
    {
        m_file->flush();
        m_file->close();
    }
    else
    {
        m_file->remove();
    }

    delete m_file;
    m_file = nullptr;
    reply->deleteLater();
}

void Downloader::SetDataName(QString name)
{
    fileName = name;
}

QString Downloader::getDataName()
{
    return fileName;
}