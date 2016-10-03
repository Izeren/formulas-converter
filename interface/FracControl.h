#pragma once

#include "CRect.h"
#include "IChild.h"
#include <memory>
#include <string>
#include <list>

class FracControl : public IChild {
public:
	FracControl(const CRect& rect, std::weak_ptr<IChild> parent);

	std::list<std::shared_ptr<IChild>> GetChildren() const;
	void SetRect(const CRect& rect);
	void Resize();

private:
	std::shared_ptr<IChild> firstChild;
	std::shared_ptr<IChild> secondChild;
};