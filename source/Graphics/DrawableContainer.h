#ifndef DRAWABLECONTAINER_H_
#define DRAWABLECONTAINER_H_
#include <vector>
#include <memory>
#include "Drawable.h"

using std::unique_ptr;
/**
 *  Base class for objects that can contain additional drawables
 *  DrawableContainer is responsible for deletion of child drawables
 */
class DrawableContainer
{
public:
	DrawableContainer();
	virtual ~DrawableContainer();

	/// Adds Drawable to the container
	void AddChild(unique_ptr<Drawable>&& child);

	/// Removes Drawable and frees memory
	bool DeleteChild(Drawable* child);

	/// Removes all Drawables and frees the memory
	void Clear();
protected:
	std::vector<unique_ptr<Drawable>> children;

	/// Called each time child is added
	virtual void OnChildAdd(Drawable* child);

	///Called when one or multiple children are removed
	virtual void OnChildRemove();
};
#endif /* DRAWABLECONTAINER_H_ */
