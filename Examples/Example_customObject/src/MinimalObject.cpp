#include "MinimalObject.h"

OBJECT_IMPL(MinimalObject);

MinimalObject::MinimalObject()
	: GameObject("MinimalObject")
{

}
MinimalObject::MinimalObject(const MinimalObject& other)
	: GameObject(other)
{

}
MinimalObject::~MinimalObject()
{
	// Do not delete components which are added to this object.
	// They will be deleted automatically.
}
void MinimalObject::update()
{
	logInfo("Update from the minimum object");
}