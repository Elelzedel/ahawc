#include "MessageQueue.hpp"

void MessageQueue::put(const char message[64]) {
	// If the queue is full, discard the message
	if (this -> index >= QUEUE_LEN - 1) {
		return;
	}

	strncpy(this -> messages[this -> index], message, 63);

	this -> index++;
}

void MessageQueue::serialPrintAll() {
	for(unsigned int i = 0; i < this -> index; i++) {
		Serial.println(this -> messages[i]);
		Serial.flush();
	}
}

void MessageQueue::clear() {
	// Don't bother clearing the messages array, just allow it to be overwritten
	this -> index = 0;
}
