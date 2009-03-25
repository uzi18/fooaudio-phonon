/****************************************************************************
** Meta object code from reading C++ file 'foomainwindow.hpp'
**
** Created: Wed Mar 25 08:03:16 2009
**      by: The Qt Meta Object Compiler version 61 (Qt 4.5.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "foomainwindow.hpp"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'foomainwindow.hpp' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 61
#error "This file was generated using the moc from 4.5.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_FooMainWindow[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
      59,   12, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

 // signals: signature, parameters, type, tag, flags
      15,   14,   14,   14, 0x05,
      28,   14,   14,   14, 0x05,
      41,   14,   14,   14, 0x05,
      54,   14,   14,   14, 0x05,
      67,   14,   14,   14, 0x05,

 // slots: signature, parameters, type, tag, flags
      81,   14,   14,   14, 0x08,
      88,   14,   14,   14, 0x08,
     102,   14,   14,   14, 0x08,
     113,   14,   14,   14, 0x08,
     125,   14,   14,   14, 0x08,
     139,   14,   14,   14, 0x08,
     153,   14,   14,   14, 0x08,
     168,   14,   14,   14, 0x08,
     183,   14,   14,   14, 0x08,
     197,   14,   14,   14, 0x08,
     204,   14,   14,   14, 0x08,
     211,   14,   14,   14, 0x08,
     218,   14,   14,   14, 0x08,
     226,   14,   14,   14, 0x08,
     238,   14,   14,   14, 0x08,
     247,   14,   14,   14, 0x08,
     259,   14,   14,   14, 0x08,
     269,   14,   14,   14, 0x08,
     287,   14,   14,   14, 0x08,
     301,   14,   14,   14, 0x08,
     321,   14,   14,   14, 0x08,
     335,   14,   14,   14, 0x08,
     344,   14,   14,   14, 0x08,
     363,   14,   14,   14, 0x08,
     381,   14,   14,   14, 0x08,
     395,   14,   14,   14, 0x08,
     405,   14,   14,   14, 0x08,
     417,   14,   14,   14, 0x08,
     435,   14,   14,   14, 0x08,
     448,   14,   14,   14, 0x08,
     474,   14,   14,   14, 0x08,
     493,   14,   14,   14, 0x08,
     499,   14,   14,   14, 0x08,
     506,   14,   14,   14, 0x08,
     514,   14,   14,   14, 0x08,
     521,   14,   14,   14, 0x08,
     529,   14,   14,   14, 0x08,
     536,   14,   14,   14, 0x08,
     547,   14,   14,   14, 0x08,
     554,   14,   14,   14, 0x08,
     563,   14,   14,   14, 0x08,
     578,   14,   14,   14, 0x08,
     595,   14,   14,   14, 0x08,
     609,   14,   14,   14, 0x08,
     623,   14,   14,   14, 0x08,
     639,   14,   14,   14, 0x08,
     655,   14,   14,   14, 0x08,
     672,   14,   14,   14, 0x08,
     691,   14,   14,   14, 0x08,
     715,   14,   14,   14, 0x08,
     739,   14,   14,   14, 0x08,
     751,   14,   14,   14, 0x08,
     765,   14,   14,   14, 0x08,
     777,   14,   14,   14, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_FooMainWindow[] = {
    "FooMainWindow\0\0playSignal()\0prevSignal()\0"
    "stopSignal()\0nextSignal()\0pauseSignal()\0"
    "open()\0openAudioCD()\0addFiles()\0"
    "addFolder()\0addLocation()\0newPlaylist()\0"
    "loadPlaylist()\0savePlaylist()\0"
    "preferences()\0exit()\0undo()\0redo()\0"
    "clear()\0selectAll()\0sortBy()\0randomize()\0"
    "reverse()\0sortByFilePatch()\0sortByAlbum()\0"
    "sortByTrackNumber()\0sortByTitle()\0"
    "search()\0removeDuplicates()\0"
    "removeDeadItems()\0alwaysOnTop()\0"
    "console()\0equalizer()\0playlistManager()\0"
    "quickSetup()\0enableLayoutEditionMode()\0"
    "createScratchbox()\0cut()\0copy()\0paste()\0"
    "stop()\0pause()\0play()\0previous()\0"
    "next()\0random()\0defaultOrder()\0"
    "repeatPlaylist()\0repeatTrack()\0"
    "randomOrder()\0shuffleTracks()\0"
    "shuffleAlbums()\0shuffleFolders()\0"
    "stopAfterCurrent()\0playbackFollowsCursor()\0"
    "cursorFollowsPlayback()\0albumList()\0"
    "searchAlbum()\0configure()\0about()\0"
};

const QMetaObject FooMainWindow::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_FooMainWindow,
      qt_meta_data_FooMainWindow, 0 }
};

const QMetaObject *FooMainWindow::metaObject() const
{
    return &staticMetaObject;
}

void *FooMainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_FooMainWindow))
        return static_cast<void*>(const_cast< FooMainWindow*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int FooMainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: playSignal(); break;
        case 1: prevSignal(); break;
        case 2: stopSignal(); break;
        case 3: nextSignal(); break;
        case 4: pauseSignal(); break;
        case 5: open(); break;
        case 6: openAudioCD(); break;
        case 7: addFiles(); break;
        case 8: addFolder(); break;
        case 9: addLocation(); break;
        case 10: newPlaylist(); break;
        case 11: loadPlaylist(); break;
        case 12: savePlaylist(); break;
        case 13: preferences(); break;
        case 14: exit(); break;
        case 15: undo(); break;
        case 16: redo(); break;
        case 17: clear(); break;
        case 18: selectAll(); break;
        case 19: sortBy(); break;
        case 20: randomize(); break;
        case 21: reverse(); break;
        case 22: sortByFilePatch(); break;
        case 23: sortByAlbum(); break;
        case 24: sortByTrackNumber(); break;
        case 25: sortByTitle(); break;
        case 26: search(); break;
        case 27: removeDuplicates(); break;
        case 28: removeDeadItems(); break;
        case 29: alwaysOnTop(); break;
        case 30: console(); break;
        case 31: equalizer(); break;
        case 32: playlistManager(); break;
        case 33: quickSetup(); break;
        case 34: enableLayoutEditionMode(); break;
        case 35: createScratchbox(); break;
        case 36: cut(); break;
        case 37: copy(); break;
        case 38: paste(); break;
        case 39: stop(); break;
        case 40: pause(); break;
        case 41: play(); break;
        case 42: previous(); break;
        case 43: next(); break;
        case 44: random(); break;
        case 45: defaultOrder(); break;
        case 46: repeatPlaylist(); break;
        case 47: repeatTrack(); break;
        case 48: randomOrder(); break;
        case 49: shuffleTracks(); break;
        case 50: shuffleAlbums(); break;
        case 51: shuffleFolders(); break;
        case 52: stopAfterCurrent(); break;
        case 53: playbackFollowsCursor(); break;
        case 54: cursorFollowsPlayback(); break;
        case 55: albumList(); break;
        case 56: searchAlbum(); break;
        case 57: configure(); break;
        case 58: about(); break;
        default: ;
        }
        _id -= 59;
    }
    return _id;
}

// SIGNAL 0
void FooMainWindow::playSignal()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void FooMainWindow::prevSignal()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}

// SIGNAL 2
void FooMainWindow::stopSignal()
{
    QMetaObject::activate(this, &staticMetaObject, 2, 0);
}

// SIGNAL 3
void FooMainWindow::nextSignal()
{
    QMetaObject::activate(this, &staticMetaObject, 3, 0);
}

// SIGNAL 4
void FooMainWindow::pauseSignal()
{
    QMetaObject::activate(this, &staticMetaObject, 4, 0);
}
QT_END_MOC_NAMESPACE
