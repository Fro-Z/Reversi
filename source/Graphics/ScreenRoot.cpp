#include "ScreenRoot.h"
ScreenRoot::ScreenRoot()
{

}
void ScreenRoot::Draw()
{
	for (auto& child : children)
	{
		child->Draw();
	}
	SetDirty(false);
}
void ScreenRoot::OnChildAdd(Drawable* child)
{
	child->SetParent(*this);
	SetDirty(true);
}
void ScreenRoot::OnChildRemove()
{
	SetDirty(true);
}
