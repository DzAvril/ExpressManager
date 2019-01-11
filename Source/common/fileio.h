#ifndef FILEIO_H
#define FILEIO_H

#include <QObject>

class FileIo : public QObject {
    Q_OBJECT
  public:
    bool IsPathExist(const QString &path);
    bool CreateDir(const QString &path);
    static FileIo *getInstance();
    Q_INVOKABLE const QString getTempPath();
    bool DeleteFile(const QString &path);
    static const QString GetCurrentPath();
  private:
    explicit FileIo(QObject *parent = nullptr);
    FileIo(const FileIo &);
    FileIo &operator=(const FileIo &);
    static FileIo *instance;

  signals:

  public slots:
};

#endif // FILEIO_H
