#include "CSintacticVisitorResults.h"

CSintacticVisitorResults::CSintacticVisitorResults(bool validationStatus) {
	this->validationStatus = validationStatus;
}

bool CSintacticVisitorResults::isValidated() {
	return validationStatus;
}