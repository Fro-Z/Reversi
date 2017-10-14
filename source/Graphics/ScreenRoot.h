#ifndef SCREENROOT_H_
#define SCREENROOT_H_
#include "Drawable.h"
#include "DrawableContainer.h"
/**
 * Root element for all Drawable objects
 */
class ScreenRoot: public Drawable, public DrawableContainer
{
public:
	ScreenRoot();
	virtual void Draw() override;

protected:
	void OnChildAdd(Drawable* child) override;
	void OnChildRemove() override;
};

#endif /* SCREENROOT_H_ */
