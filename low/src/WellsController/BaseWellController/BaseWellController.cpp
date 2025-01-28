#include "BaseWellController.hpp"

bool BaseWellController::getIsActive() {
	return this -> isActive;
}

bool BaseWellController::isBelowCutoffCurrent() {
	// Skip this check if the well just started
	if (millis() < this -> warmup) {
		return false;
	}

	// Sensor 0 is the tank pump, offset so that well 0 is sensor 1
	float current = this -> currentObserver -> getCurrent(this -> id + 1);

	if (current < this -> cutoffCurrent) {
		this -> consecutiveUndercurrents++; 
	} else {
		this -> consecutiveUndercurrents = 0; 
	}

	if (consecutiveUndercurrents > MAX_CONSECUTIVE_UNDERCURRENTS) {
		return true;
	}

	return false;
}


bool BaseWellController::isMeetingMinRiseRate() {
	if (millis() < this -> warmup) {
		return true;
	}

	// Still waiting to do the next sample
	if (millis() < this -> nextRiseRateSample) {
		return true;
	}

	short newLevel = this -> tankLevelObserver -> getLevel();
	short levelChange = newLevel - this -> lastLevel;
	this -> lastLevel = newLevel;
	this -> nextRiseRateSample = millis() + RISE_RATE_SAMPLE_MS; 

	if (levelChange < this -> minRiseRate) {
		return false;
	}
	return true;
}
