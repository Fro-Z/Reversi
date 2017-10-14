#include "DrawableContainer.h"
DrawableContainer::DrawableContainer()
{

}
DrawableContainer::~DrawableContainer()
{
	Clear();
}
void DrawableContainer::AddChild(unique_ptr<Drawable>&& child)
{
	children.emplace_back(std::move(child));
	OnChildAdd(children.back().get());
}
bool DrawableContainer::DeleteChild(Drawable* child)
{
	for (auto it = children.begin(); it != children.end(); ++it)
	{
		if (child == (*it).get())
		{
			children.erase(it);
			OnChildRemove();
			return true;
		}
	}
	return false;
}

void DrawableContainer::OnChildAdd(Drawable* child)
{
	//no default behavior
}
void DrawableContainer::Clear()
{
	children.clear();
	OnChildRemove();
}
void DrawableContainer::OnChildRemove()
{

}
