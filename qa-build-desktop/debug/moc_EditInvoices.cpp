/****************************************************************************
** Meta object code from reading C++ file 'EditInvoices.h'
**
** Created: Mon Jan 2 23:01:54 2012
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../src/EditInvoices.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'EditInvoices.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_EditInvoices[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

       0        // eod
};

static const char qt_meta_stringdata_EditInvoices[] = {
    "EditInvoices\0"
};

const QMetaObject EditInvoices::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_EditInvoices,
      qt_meta_data_EditInvoices, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &EditInvoices::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *EditInvoices::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *EditInvoices::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_EditInvoices))
        return static_cast<void*>(const_cast< EditInvoices*>(this));
    return QDialog::qt_metacast(_clname);
}

int EditInvoices::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
QT_END_MOC_NAMESPACE
