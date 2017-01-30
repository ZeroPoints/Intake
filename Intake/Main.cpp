#include <windows.h>
#include <vector>
#include <al.h>
#include <alc.h>


#define FREQ 44100   // Sample rate
#define BUFFERSIZE 1024




int main(int argc, char *argv[]) {


	//input vars
	ALCint inputSample = 0;
	ALCdevice *inputDevice;
	std::vector<ALbyte> intermediateBuffer;
	//This input buffer has to stay really big compared to the buffersize 10 seems reasonable enough
	//Might be a better variable for this
	ALubyte inputBuffer[BUFFERSIZE*10];


	//output vars
	ALuint outputSource;
	ALuint outputBuffer;
	ALCcontext *outputContext;
	ALCdevice *outputDevice;
	ALint outputSourceState;


	//found this in some code online
	ALenum error;
	 
	//input prep
	inputDevice = alcCaptureOpenDevice(NULL, FREQ, AL_FORMAT_STEREO16, BUFFERSIZE);
	if (!inputDevice) {
		printf("inputDevice error");
	}
	


	//output prep
	outputDevice = alcOpenDevice(NULL);
	if (!outputDevice) {
		printf("outputDevice error");
	}
	outputContext = alcCreateContext(outputDevice, NULL);
	if (!alcMakeContextCurrent(outputContext)) {
		printf("alcMakeContextCurrent error");
	}
	alGenSources(1, &outputSource);
	if ((error = alGetError()) != AL_NO_ERROR)
	{
		printf("outputSource : %d", error);
	}
	alGenBuffers(1, &outputBuffer);
	if ((error = alGetError()) != AL_NO_ERROR)
	{
		printf("outputBuffer : %d", error);
	}




	//Start capturing input
	alcCaptureStart(inputDevice);
	while (true) {
		alcGetIntegerv(inputDevice, ALC_CAPTURE_SAMPLES, 1, &inputSample);
		if (inputSample > BUFFERSIZE) {
			alcCaptureSamples(inputDevice, inputBuffer, inputSample);

			//Input samples size for different AUDIO samples
			//Mono8 = 1Byte
			//Mono16 = 2Bytes
			//Stereo8 = 2Bytes
			//Stereo16 = 4Bytes
			//insert all data into my mega buffer 
			intermediateBuffer.insert(intermediateBuffer.end(), &inputBuffer[0], &inputBuffer[inputSample*4]);
		}
		auto keyPressed = GetAsyncKeyState(VK_ESCAPE);
		if (keyPressed)
		{
			break;
		}
	}

	//grab the first address of buffer
	//might be a neater way of doing this
	ALbyte* inputBufferByteAddress = &intermediateBuffer[0];
	alBufferData(outputBuffer, AL_FORMAT_STEREO16, (ALvoid *)inputBufferByteAddress, intermediateBuffer.size(), FREQ);                    //Store the sound data in the OpenAL Buffer

	auto res = alGetError();
	if (alGetError() != AL_NO_ERROR) 
	{
		printf("loading al buffer error");
		system("PAUSE");
		return 0;
	}                                


																				
	//Sound setting variables
	//Position of the source sound
	ALfloat SourcePos[] = { 0.0, 0.0, 0.0 };
	//Velocity of the source sound
	ALfloat SourceVel[] = { 0.0, 0.0, 0.0 };    
	//Position of the listener
	ALfloat ListenerPos[] = { 0.0, 0.0, 0.0 };               
	//Velocity of the listener
	ALfloat ListenerVel[] = { 0.0, 0.0, 0.0 };                        
	//Orientation of the listener
	ALfloat ListenerOri[] = { 0.0, 0.0, -1.0,  0.0, 1.0, 0.0 };                 
		
	//Listener       
	//First direction vector, then vector pointing up) 			
	//Set position of the listener
	alListenerfv(AL_POSITION, ListenerPos);        
	//Set velocity of the listener
	alListenerfv(AL_VELOCITY, ListenerVel);             
	//Set orientation of the listener
	alListenerfv(AL_ORIENTATION, ListenerOri);                                  


	//Link the buffer to the source
	alSourcei(outputSource, AL_BUFFER, outputBuffer);  
	//Set the pitch of the source
	alSourcef(outputSource, AL_PITCH, 1.0f);

	//Set the gain of the source
	alSourcef(outputSource, AL_GAIN, 1.0f);
	//Set the position of the source
	alSourcefv(outputSource, AL_POSITION, SourcePos);
	//Set the velocity of the source
	alSourcefv(outputSource, AL_VELOCITY, SourceVel);
	//Set if source is looping sound
	alSourcei(outputSource, AL_LOOPING, AL_FALSE);


	//Actually play the input
	alSourcePlay(outputSource);


	ALubyte c = ' ';
	while (c != 'q')
	{
		alGetSourcei(outputSource, AL_SOURCE_STATE, &outputSourceState);
		if (outputSourceState == AL_PLAYING) {
			printf("currently playing\n");
		}

		c = getchar();

		switch (c)
		{
			//force next pass for state recheck
		case 'n':
		{
			break;
		}

		// Pressing 'p' will begin playing the sample.
		case 'p':
		{
			alSourcePlay(outputSource);
			break;
		}

		// Pressing 's' will stop the sample from playing.
		case 's':
		{
			alSourceStop(outputSource);
			break;
		}

		// Pressing 'h' will pause (hold) the sample.
		case 'h':
		{
			alSourcePause(outputSource);
			break;
		}
		};
	}


	return 0;
}




