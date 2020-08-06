#include "vcaudio.h"

VCAudio::VCAudio(QObject *parent) : QObject(parent)
{
    initializeAudio();
}

VCAudio::~VCAudio()
{
    exitThread = true;
}

void VCAudio::registerQMLType()
{
    qmlRegisterType<VCAudio>("VCAudio", 1, 0, "VCAudio");
}

void VCAudio::appendData(QString data)
{
    outputQueue.append(data);
}

void VCAudio::readAudio()
{
    QByteArray data = inputBuffer->data();
    int length = data.length();
    int bytesToRead = length - prevPos;

    QByteArray readBytes = data.right(bytesToRead);
    prevPos = length;

    if(!readBytes.isEmpty()){
        emit audioDataReady(QString::fromStdString(readBytes.toBase64().toStdString()));
    }else {
        qDebug() << "-- empty...";
    }

}

void VCAudio::printInputState(QAudio::State state)
{
    qDebug() << "input state is: " << state;
}

void VCAudio::printOutputState(QAudio::State state)
{
    qDebug() << "output state is: " << state;
}

void VCAudio::initializeAudio()
{

    //request permission
    //requestAudioPermission();

    QAudioFormat audioFormat;
    audioFormat.setSampleRate(8000);
    audioFormat.setChannelCount(1);
    audioFormat.setSampleSize(16);
    audioFormat.setSampleType(QAudioFormat::UnSignedInt);
    audioFormat.setByteOrder(QAudioFormat::LittleEndian);
    audioFormat.setCodec("audio/mpeg");

    QAudioDeviceInfo infoIn = QAudioDeviceInfo::defaultInputDevice();

    if(!infoIn.isFormatSupported(audioFormat)){
        audioFormat = infoIn.nearestFormat(audioFormat);
    }

    QAudioDeviceInfo infoOut(QAudioDeviceInfo::defaultOutputDevice());

    if(!infoOut.isFormatSupported(audioFormat)){
        audioFormat = infoOut.nearestFormat(audioFormat);
    }

    audioInput = new QAudioInput(audioFormat,this);
    audioOutput = new QAudioOutput(infoOut,audioFormat,this);

    connect(audioInput, SIGNAL(stateChanged(QAudio::State)), this, SLOT(printInputState(QAudio::State)));
    connect(audioOutput, SIGNAL(stateChanged(QAudio::State)), this, SLOT(printOutputState(QAudio::State)));

    inputBuffer = new QBuffer(this);
    inputBuffer->open(QIODevice::ReadWrite | QIODevice::Truncate);
    audioInput->start(inputBuffer);
    connect(inputBuffer, SIGNAL(readyRead()), this, SLOT(readAudio()));

    outputIODevice = audioOutput->start();
    //worker thread to keep audio output active
    workerThread = QtConcurrent::run(this, &VCAudio::outputWork);

}

void VCAudio::outputWork()
{
    while(!exitThread){

        QByteArray byteArray;
        QByteArray base64;
        QString data;

        if(outputQueue.length() > 0){
            data = outputQueue.at(0);
            outputQueue.remove(0);
            base64.clear();
            base64.append(data);
            byteArray.clear();
            byteArray = QByteArray::fromBase64(base64);
            outputIODevice->write(byteArray);
        }

    }
}

#ifdef Q_OS_ANDROID
void VCAudio::requestAudioPermission()
{
    QAndroidJniObject permission = QAndroidJniObject::fromString("android.permission.RECORD_AUDIO");

    QAndroidJniObject::callStaticMethod<void>(
                "com/bunistack/utils/PermissionsUtils",
                "requestPermission",
                "(Landroid/app/Activity;Landroid/content/Context;Ljava/lang/String;)V",
                QtAndroid::androidActivity().object(),
                QtAndroid::androidContext().object(),
                permission.object<jstring>());
}
#endif
