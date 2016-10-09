#include "IExportVisitor.h"


void IExportVisitor::ClearVisitor()
{
	this->description = "";
	this->lastVisited = 0;
	this->priorities = std::stack<LSVUtils::TPriority>();
	this->priorities.push(LSVUtils::TPriority::EMPTY_FORMULA);
}