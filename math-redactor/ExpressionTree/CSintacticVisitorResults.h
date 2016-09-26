#pragma once

#include "IVisitorResult.h"

class CSintacticVisitorResults : public IVisitorResult {

public:
	CSintacticVisitorResults(bool validationStatus=false);
	bool isValidated();

private:
	bool validationStatus;
};