#ifndef MESSAGE_QUEUE_HPP
#define MESSAGE_QUEUE_HPP

#include <Arduino.h>

class MessageQueue {
	public:
		void put(const char message[64]);
		void serialPrintAll();
		void clear();

	private:
		static const unsigned int QUEUE_LEN = 32;
		static const unsigned int MESSAGE_LEN = 64;
		char messages[QUEUE_LEN][MESSAGE_LEN];
		unsigned int index = 0;
};

#endif
