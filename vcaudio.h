#ifndef VCAUDIO_H
#define VCAUDIO_H

#include <QObject>
#include <QQmlEngine>
#include <QAudioRecorder>
#include <QAudioProbe>
#include <QAudioOutput>
#include <QAudioInput>
#include <QBuffer>
#include <QFile>
#include <QFuture>
#include <QtConcurrent/QtConcurrent>

#ifdef Q_OS_ANDROID
    #include <QtAndroidExtras/QAndroidJniObject>
    #include <QtAndroidExtras/QtAndroid>
#endif

class VCAudio : public QObject
{
    Q_OBJECT
public:
    explicit VCAudio(QObject *parent = nullptr);
    virtual ~VCAudio();

    static void registerQMLType();

    Q_INVOKABLE void appendData(QString data);

signals:
    void audioDataReady(QString data);    

public slots:
    void readAudio();
    void printInputState(QAudio::State state);
    void printOutputState(QAudio::State state);

private:
    QAudioOutput *audioOutput;
    QAudioInput *audioInput;
    QBuffer *inputBuffer;
    QIODevice *outputIODevice;
    QFuture<void> workerThread;
    QVector<QString> outputQueue;
    bool exitThread = false;
    int prevPos = 0;

    QFile *audioFile;

    void initializeAudio();
    void outputWork();

#ifdef Q_OS_ANDROID
    void requestAudioPermission();
#endif

};

#endif // VCAUDIO_H
