/****************************************************************************
** Meta object code from reading C++ file 'foomainwindow.hpp'
**
** Created: Tue Mar 24 00:10:48 2009
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
      54,   12, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

 // slots: signature, parameters, type, tag, flags
      15,   14,   14,   14, 0x08,
      22,   14,   14,   14, 0x08,
      36,   14,   14,   14, 0x08,
      47,   14,   14,   14, 0x08,
      59,   14,   14,   14, 0x08,
      73,   14,   14,   14, 0x08,
      87,   14,   14,   14, 0x08,
     102,   14,   14,   14, 0x08,
     117,   14,   14,   14, 0x08,
     131,   14,   14,   14, 0x08,
     138,   14,   14,   14, 0x08,
     145,   14,   14,   14, 0x08,
     152,   14,   14,   14, 0x08,
     160,   14,   14,   14, 0x08,
     172,   14,   14,   14, 0x08,
     181,   14,   14,   14, 0x08,
     193,   14,   14,   14, 0x08,
     203,   14,   14,   14, 0x08,
     221,   14,   14,   14, 0x08,
     235,   14,   14,   14, 0x08,
     255,   14,   14,   14, 0x08,
     269,   14,   14,   14, 0x08,
     278,   14,   14,   14, 0x08,
     297,   14,   14,   14, 0x08,
     315,   14,   14,   14, 0x08,
     329,   14,   14,   14, 0x08,
     339,   14,   14,   14, 0x08,
     351,   14,   14,   14, 0x08,
     369,   14,   14,   14, 0x08,
     382,   14,   14,   14, 0x08,
     408,   14,   14,   14, 0x08,
     427,   14,   14,   14, 0x08,
     433,   14,   14,   14, 0x08,
     440,   14,   14,   14, 0x08,
     448,   14,   14,   14, 0x08,
     455,   14,   14,   14, 0x08,
     463,   14,   14,   14, 0x08,
     470,   14,   14,   14, 0x08,
     481,   14,   14,   14, 0x08,
     488,   14,   14,   14, 0x08,
     497,   14,   14,   14, 0x08,
     512,   14,   14,   14, 0x08,
     529,   14,   14,   14, 0x08,
     543,   14,   14,   14, 0x08,
     557,   14,   14,   14, 0x08,
     573,   14,   14,   14, 0x08,
     589,   14,   14,   14, 0x08,
     606,   14,   14,   14, 0x08,
     625,   14,   14,   14, 0x08,
     649,   14,   14,   14, 0x08,
     673,   14,   14,   14, 0x08,
     685,   14,   14,   14, 0x08,
     699,   14,   14,   14, 0x08,
     711,   14,   14,   14, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_FooMainWindow[] = {
    "FooMainWindow\0\0open()\0openAudioCD()\0"
    "addFiles()\0addFolder()\0addLocation()\0"
    "newPlaylist()\0loadPlaylist()\0"
    "savePlaylist()\0preferences()\0exit()\0"
    "undo()\0redo()\0clear()\0selectAll()\0"
    "sortBy()\0randomize()\0reverse()\0"
    "sortByFilePatch()\0sortByAlbum()\0"
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
        case 0: open(); break;
        case 1: openAudioCD(); break;
        case 2: addFiles(); break;
        case 3: addFolder(); break;
        case 4: addLocation(); break;
        case 5: newPlaylist(); break;
        case 6: loadPlaylist(); break;
        case 7: savePlaylist(); break;
        case 8: preferences(); break;
        case 9: exit(); break;
        case 10: undo(); break;
        case 11: redo(); break;
        case 12: clear(); break;
        case 13: selectAll(); break;
        case 14: sortBy(); break;
        case 15: randomize(); break;
        case 16: reverse(); break;
        case 17: sortByFilePatch(); break;
        case 18: sortByAlbum(); break;
        case 19: sortByTrackNumber(); break;
        case 20: sortByTitle(); break;
        case 21: search(); break;
        case 22: removeDuplicates(); break;
        case 23: removeDeadItems(); break;
        case 24: alwaysOnTop(); break;
        case 25: console(); break;
        case 26: equalizer(); break;
        case 27: playlistManager(); break;
        case 28: quickSetup(); break;
        case 29: enableLayoutEditionMode(); break;
        case 30: createScratchbox(); break;
        case 31: cut(); break;
        case 32: copy(); break;
        case 33: paste(); break;
        case 34: stop(); break;
        case 35: pause(); break;
        case 36: play(); break;
        case 37: previous(); break;
        case 38: next(); break;
        case 39: random(); break;
        case 40: defaultOrder(); break;
        case 41: repeatPlaylist(); break;
        case 42: repeatTrack(); break;
        case 43: randomOrder(); break;
        case 44: shuffleTracks(); break;
        case 45: shuffleAlbums(); break;
        case 46: shuffleFolders(); break;
        case 47: stopAfterCurrent(); break;
        case 48: playbackFollowsCursor(); break;
        case 49: cursorFollowsPlayback(); break;
        case 50: albumList(); break;
        case 51: searchAlbum(); break;
        case 52: configure(); break;
        case 53: about(); break;
        default: ;
        }
        _id -= 54;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
