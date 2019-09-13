#ifndef ABSTRACTOPTIONEDITOR_H
#define ABSTRACTOPTIONEDITOR_H

#include <QVariant>

class AbstractOptionEditor
{
    QVariant _value;
public:
    AbstractOptionEditor();
    virtual ~AbstractOptionEditor();

    virtual void reset();

    virtual QVariant value() const;
    virtual void setValue(const QVariant &value);
    virtual void setDropDown(const QStringList &list);
};

#endif // ABSTRACTOPTIONEDITOR_H