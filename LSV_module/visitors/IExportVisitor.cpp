#include "IExportVisitor.h"


void IExportVisitor::ClearVisitor()
{
	this->description = "";
	this->lastVisited = 0;
	this->priorities = std::stack<LSVUtils::TPriority>();
}