#pragma once
#include "QSFML_EditorWidget.h"
#include <QObject>

class MinimalObject : public QSFML::Objects::GameObject
{
    OBJECT_DECL(MinimalObject);
public:
    MinimalObject();
    MinimalObject(const MinimalObject& other);

    ~MinimalObject() override;

protected:
    void update() override;

private:

};