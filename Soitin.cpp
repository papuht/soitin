#include <iostream>
#include <iomanip>
#include <thread>
#include <string>
#include "SerialPort.h"
#include "sound.h"
#include <SFML/Audio.hpp>
#include <SFML/System/MemoryInputStream.hpp>
#include <vector>

char inputData[INPUT_DATA_BYTES];
double inputValFreq = 0.0;
double inputAmp = 0.0;
double inputVal = 0.0;
double modInputVal = 0.0;
char comport[] = "COM4";
char* port = comport;

int main() {

	SerialPort arduino(port);
	
	sf::Sound sound;
	std::vector<sf::Int16> samples;
	int i = 0;
	sf::Int16 sample_start = 0;
	sf::Int16 sample_stop = samples.size();
	//const void* input = 0;

	if (arduino.isConnected()) {
		std::cout << "Connected to " << port << std::endl;
	}
	else {
		std::cin.get();
	}

	while (arduino.isConnected()) {

		
		
		arduino.ReadSerialPort(inputData, INPUT_DATA_BYTES);

		
		std::string inputValStr(inputData);

		sf::SoundBuffer buffer;
		try {
			
			inputValFreq = std::stod(inputValStr);
			if (inputValFreq > 330) {
				inputVal = inputValFreq - 300.0;
			}
			else {
				inputVal = inputValFreq;
			}
			if (inputVal < 400) {
				if (inputVal > 300) {
					modInputVal = inputVal * 1.5;
				}
				else if (inputVal < 100) {
					modInputVal = inputVal + 300.0;
				}
			}
			else {
				modInputVal = inputVal;
			}



			if (modInputVal < 330) {
				inputAmp = 0.4;
			}
			else if (modInputVal > 330 && modInputVal < 400) {
				inputAmp = 0.7;
			}
			else if (modInputVal > 400) {
				inputAmp = 0.9;
			}

			//const void* input = inputData;
			i++;

			samples.push_back(sound::SineWave(44100, modInputVal, inputAmp));
			//sf::MemoryInputStream stream; 
			//stream.open(input, 8);

			
			//buffer.loadFromStream(stream);
			//buffer.loadFromSamples(&samples[0], samples.size(), 1, 10000);
			

			


		}
		catch (...) {
			//input = 0;
			inputValFreq = 0.0;
			//i++;
		}
		
		
			
			/*
			if (i > 500) {
				samples.clear();
				i = 0;
			}
			*/
			
		
		if (buffer.loadFromSamples(&sample_start, sample_stop, 1, 10000)) {
			sound.setBuffer(buffer);
			
			
		}
		sf::Sound::Status Status = sound.getStatus();
		if (Status == Playing) {
			sound.play();
			sample_start = sample_stop;
			sample_stop = samples.size();
		}
		else {
			std::this_thread::sleep_for(std::chrono::milliseconds(1000));
		}
		
		//std::cout << std::fixed << std::setprecision(2);
		//std::cout << inputValDbl << '\r';
		std::cout << "Frekvenssi:" << modInputVal << '\n';
		std::cout << "Bufferissa:" << buffer.getSampleCount() << '\n';
		std::cout << "Aloituskohta:" << sample_start << '\n';
		std::cout << "Lopetuskohta:" << sample_stop << '\n';
		std::cout << "Sound is:" << sound.getStatus() << '\n';
		


		std::this_thread::sleep_for(std::chrono::milliseconds(10));
		
		


	}

	
	
	return 0;
}
