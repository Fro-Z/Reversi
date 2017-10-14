#ifndef DRAWABLE_H_
#define DRAWABLE_H_
/**
 * Base class for drawable objects displayed in terminal (windows etc.)
 */
class Drawable
{
public:
	virtual ~Drawable();
	Drawable();

	///Draw the Drawable in terminal
	virtual void Draw()=0;

	///If the Drawable is in Dirty state it requires re-Drawing
	inline bool IsDirty() const
	{
		return dirtyFlag;
	}
	///Set dirty state
	void SetDirty(bool isDirty = true);
protected:
	//ScreenRoot and MainMenu need to call SetParent of Drawable
	friend class ScreenRoot;
	friend class MainMenu;

	inline void SetParent(Drawable& newParent)
	{
		parent=&newParent;
	}
	inline Drawable* GetParent() const
	{
		return parent;
	}

private:
	/// Draw() needs to be called to show updated state
	bool dirtyFlag;

	Drawable* parent;
};


#endif /* DRAWABLE_H_ */
