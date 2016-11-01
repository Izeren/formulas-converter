#include "FracControl.h"
#include <algorithm>

FracControl::FracControl(const CRect& rect, std::weak_ptr<IChild> parent) :
	IChild(rect, parent)
{
	this->rect.Set(0, 0, 0, rect.GetHeight());
}

void FracControl::Resize()
{
	int width = firstChild->GetRect().GetWidth() > secondChild->GetRect().GetWidth() ? firstChild->GetRect().GetWidth() : secondChild->GetRect().GetWidth();
	int height = firstChild->GetRect().GetHeight() + secondChild->GetRect().GetHeight() + 5;

	rect.Right() = rect.Left() + width;
	rect.Bottom() = rect.Top() + height;
}

std::list<std::shared_ptr<IChild>> FracControl::GetChildren() const
{
	return std::list<std::shared_ptr<IChild>> { firstChild, secondChild };
}

void FracControl::SetRect(const CRect& _rect)
{
	this->rect = _rect;
}