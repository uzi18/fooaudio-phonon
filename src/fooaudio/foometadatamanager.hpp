#ifndef FOOMETADATAMANAGER_HPP
#define FOOMETADATAMANAGER_HPP

#include <QList>
#include <QObject>

class QTimer;
class FooTrack;

class FooMetaDataManager : public QObject
{
  
	Q_OBJECT

	static FooMetaDataManager *Instance;

	QList<FooTrack *> Tracks;
	FooTrack *CurrentTrack;
	QTimer *Refresh;

	FooMetaDataManager();
	void init();

public:
	~FooMetaDataManager();

	static FooMetaDataManager* instance();

	void addTrack(FooTrack *track);
	void deleteTrack(FooTrack *track);
	QList<FooTrack*> tracks();
	FooTrack* track(int number) { return Tracks.at(number);}
	int track(FooTrack* track) { return Tracks.indexOf(track);}
	void clear();

private slots:
	void timeout();
};

#endif // FOOMETADATAMANAGER_HPP
