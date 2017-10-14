#include "Drawable.h"


Drawable::Drawable():dirtyFlag(true),parent(nullptr)
{

}
Drawable::~Drawable()
{

}
void Drawable::SetDirty(bool isDirty)
{
	dirtyFlag=isDirty;

	if(dirtyFlag&&GetParent())//making drawable dirty affects all parent drawables
		GetParent()->SetDirty();
}
