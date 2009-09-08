#ifndef _FOOAUDIOENGINEPLUGIN_HPP_
#define _FOOAUDIOENGINEPLUGIN_HPP_

#include <QUrl>

class FooAudioEnginePlugin : public QObject
{
	Q_OBJECT

public:
	FooAudioEnginePlugin (QObject * parent = 0);

virtual	bool isPlaying() = 0;
virtual	bool isStopped() = 0;
virtual	bool isPaused()  = 0;
virtual	bool isMuted() = 0;
virtual	void setMuted(bool) = 0;
virtual	qint64 totalTime() = 0;
virtual	void seek (qint64 time) = 0;

signals:
	void aboutToFinish ();
	void progress (qint64 time);
	void willPlayNow (QUrl file);

public slots:
virtual	void stop() = 0;
virtual	void play() = 0;
virtual	void pause() = 0;
virtual	void clearQueue() = 0;
virtual	void enqueueNextFile(QUrl path) = 0;
virtual	void playFile (QUrl path) = 0;

virtual	void setVolume (int vol) = 0;
};

#endif // _FOOAUDIOENGINEPLUGIN_HPP_
