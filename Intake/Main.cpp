#include <windows.h>
#include <vector>
#include <al.h>
#include <alc.h>
#include <iostream>
#include <fstream>
#include <string.h>
#include <allegro5\allegro.h>
#include <allegro5\allegro_font.h>//fonts
#include <allegro5\allegro_ttf.h>//fonts
#include <allegro5\allegro_primitives.h>//shapes
#include <allegro5\allegro_image.h>//shapes


//http://stackoverflow.com/questions/2066090/plotting-waveform-of-the-wav-file
//https://en.wikipedia.org/wiki/Fast_Fourier_transform
//https://en.wikipedia.org/wiki/Audio_power
//https://social.msdn.microsoft.com/Forums/vstudio/en-US/7e8053b2-064f-4199-9511-f0bc41bcddb9/how-to-get-intensity-from-a-wav-file?forum=vcgeneral
//https://www.codeproject.com/kb/audio-video/img2wav.aspx
//http://codeidol.com/java/swing/Audio/Build-an-Audio-Waveform-Display/
//http://www.cplusplus.com/forum/general/109119/


#define FREQ 44100   // Sample rate
#define BUFFERSIZE 1024

void Draw(std::vector<std::vector<int>> &intDataStore);
void OpenAndListen(std::vector<std::vector<int>> &intDataStore);
void RecordAndSave();



struct WAVE_Format
{

	char chunkID[5];//"riff"(4)
	long chunkSize;//sizeof(DWORD)x1(4)
	char format[5];//"wave"(4)


	char subChunk1ID[5];//"fmt "(4)
	long subChunk1Size;//sizeof(DWORD)x1(4)
	short formatType;//(2)
	short channels;//(2)
	long sampleRate;//(4)
	long avgBytesPerSec;//(4)
	short bytesPerSample;//blockAlign?//(2)
	short bitsPerSample;//(2)


	char subChunk2ID[5]; //"data"////(4)
	long subChunk2Size; //Stores the size of the data block//(4)

	//data 
};








int main(int argc, char *argv[])
{


	std::vector<std::vector<int>> intDataStore;


	OpenAndListen(intDataStore);




	Draw(intDataStore);


	Sleep(5000);

	return 0;
}




void Draw(std::vector<std::vector<int>> &intDataStore)
{


	al_init();

	ALLEGRO_DISPLAY *display = NULL;
	ALLEGRO_EVENT_QUEUE *event_queue = NULL;
	ALLEGRO_TIMER *timer = NULL;
	display = al_create_display(1200, 1200);
	timer = al_create_timer(1.0 / 60.0);


	al_init_image_addon();
	al_install_keyboard();
	event_queue = al_create_event_queue();

	al_register_event_source(event_queue, al_get_keyboard_event_source());
	al_register_event_source(event_queue, al_get_display_event_source(display));
	al_register_event_source(event_queue, al_get_timer_event_source(timer));


	al_init_font_addon();
	al_init_ttf_addon();
	al_init_primitives_addon();


	ALLEGRO_FONT * font30_;
	font30_ = al_load_font("arial.ttf", 30, 0);

	bool redraw = false;
	bool done = false;



	al_start_timer(timer);
	ALLEGRO_EVENT ev;

	bool printlines = false;


	int previousX = 0;
	int previousY = 0;
	bool t = false;
	bool g = false;
	bool e = false;
	bool d = false;
	bool r = false;
	bool f = false;

	printf("Width=%d\n", intDataStore[0].size());
	//


	int offset = 0;
	bool rightdown = false;
	bool leftdown = false;

	/*std::ofstream pFile("datch0.txt");
	for (const auto &e : intDataStore[0])
	{
		pFile << e << "\n";
	}
	pFile.close();

	if (intDataStore.size() > 1)
	{
		pFile = std::ofstream("datch1.txt");
		for (const auto &e : intDataStore[1])
		{
			pFile << e << "\n";
		}
		pFile.close();
	}*/

	while (!done)
	{
		al_wait_for_event(event_queue, &ev);
		if (ev.type == ALLEGRO_EVENT_KEY_DOWN)
		{
			switch (ev.keyboard.keycode)
			{
			case ALLEGRO_KEY_T://choose later....
				t = true;
				break;
			case ALLEGRO_KEY_G://choose later....
				g = true;
				break;
			case ALLEGRO_KEY_E://choose later....
				e = true;
				break;
			case ALLEGRO_KEY_D://choose later....
				d = true;
				break;

			case ALLEGRO_KEY_R://choose later....
				r = true;
				break;
			case ALLEGRO_KEY_F://choose later....
				f = true;
				break;
			case ALLEGRO_KEY_LEFT://choose later....
				leftdown = true;
				break;
			case ALLEGRO_KEY_RIGHT://choose later....
				rightdown = true;
				break;
			}
		}
		if (ev.type == ALLEGRO_EVENT_KEY_UP)
		{
			switch (ev.keyboard.keycode)
			{
			case ALLEGRO_KEY_T://choose later....
				t = false;
				break;
			case ALLEGRO_KEY_G://choose later....
				g = false;
				break;
			case ALLEGRO_KEY_E://choose later....
				e = false;
				break;
			case ALLEGRO_KEY_D://choose later....
				d = false;
				break;
			case ALLEGRO_KEY_R://choose later....
				r = false;
				break;
			case ALLEGRO_KEY_F://choose later....
				f = false;
				break;
			case ALLEGRO_KEY_LEFT://choose later....
				leftdown = false;
				break;
			case ALLEGRO_KEY_RIGHT://choose later....
				rightdown = false;
				break;
			}
		}

		if (leftdown)
		{
			offset -= 50;
			if (offset <= 0)
			{
				offset = 0;
			}
		}
		if (rightdown)
		{
			offset += 50;
		}

		if (ev.type == ALLEGRO_EVENT_TIMER)
		{
			redraw = true;
		}
		else if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
		{
			done = true;
		}
		if (redraw && al_is_event_queue_empty(event_queue))
		{

			previousX = 0;
			previousY = 0;
			auto i = 1;
			auto k = 0;

			for (auto j = offset; j < 1200 + offset; j++)
			{
				if (j >= intDataStore[0].size())
				{
					break;
				}
				int x = 10 + j - offset;
				int y = 0;
				if (intDataStore[k][j] >= 127)
				{
					y = 400 + ((255 + 127 - intDataStore[k][j])*i);
				}
				else
				{
					y = 400 + ((127 - intDataStore[k][j])*i);
				}
				if (previousX == 0 || previousY == 0)
				{
					previousX = x;
					previousY = y;
					continue;
				}
				al_draw_line(previousX, previousY, x, y, al_map_rgb(255, 0, 125), 1);
				previousX = x;
				previousY = y;
				if (printlines)
				{
					printf("\n%d", intDataStore[k][j]);
				}
			}


			if (intDataStore.size() > 1)
			{

				previousX = 0;
				previousY = 0;
				i = -1;
				k = 1;
				for (auto j = offset; j < 1200 + offset; j++)
				{
					if (j >= intDataStore[0].size())
					{
						break;
					}
					int x = 10 + j - offset;
					int y = 0;
					if (intDataStore[k][j] >= 127)
					{
						y = 400 + ((255 + 127 - intDataStore[k][j])*i);
					}
					else
					{
						y = 400 + ((127 - intDataStore[k][j])*i);
					}
					if (previousX == 0 || previousY == 0)
					{
						previousX = x;
						previousY = y;
						continue;
					}
					al_draw_line(previousX, previousY, x, y, al_map_rgb(255, 0, 125), 1);
					previousX = x;
					previousY = y;
					if (printlines)
					{
						printf("\n%d", intDataStore[k][j]);
					}
				}

			}

			al_draw_textf(font30_, al_map_rgb(255, 255, 255), 10, 10, ALLEGRO_ALIGN_LEFT, "offset: %d", offset);//how many frames have been drawn

			redraw = false;
			al_flip_display();
			al_clear_to_color(al_map_rgb(0, 0, 0));
		}

	}

	al_destroy_timer(timer);
	al_destroy_display(display);
	al_destroy_event_queue(event_queue);

}





void OpenAndListen(std::vector<std::vector<int>> &intDataStore)
{


	WAVE_Format sWaveHeader;

	//output vars
	ALuint outputSource;
	ALuint outputBuffer;
	ALCcontext *outputContext;
	ALCdevice *outputDevice;
	ALint outputSourceState;


	ALenum error;



	//This is wave saving
	FILE * pFile;

	//fopen_s(&pFile, "sine.wav", "rb");
	//fopen_s(&pFile, "Alesis-Fusion-Bass-Loop.wav", "rb");
	fopen_s(&pFile, "myfile.wav", "rb");
	//myfile.wav
	fread(sWaveHeader.chunkID, sizeof(char), 4, pFile);             // "RIFF"
	if (sWaveHeader.chunkID[0] != 'R' || sWaveHeader.chunkID[1] != 'I' || sWaveHeader.chunkID[2] != 'F' || sWaveHeader.chunkID[3] != 'F')
	{
		printf("chunkID error");
		return;
	}
	fread(&sWaveHeader.chunkSize, sizeof(int), 1, pFile); // 4 + (8 + SubChunk1Size) + (8 + SubChunk2Size)

	fread(sWaveHeader.format, sizeof(char), 4, pFile); // "WAVE"
	if (sWaveHeader.format[0] != 'W' || sWaveHeader.format[1] != 'A' || sWaveHeader.format[2] != 'V' || sWaveHeader.format[3] != 'E')
	{
		printf("format error");
		return;
	}

	fread(sWaveHeader.subChunk1ID, sizeof(char), 4, pFile);// "fmt "
	if (sWaveHeader.subChunk1ID[0] != 'f' || sWaveHeader.subChunk1ID[1] != 'm' || sWaveHeader.subChunk1ID[2] != 't' || sWaveHeader.subChunk1ID[3] != ' ')
	{
		printf("subChunk1ID error");
		return;
	}

	fread(&sWaveHeader.subChunk1Size, sizeof(int), 1, pFile);// bytes remaining in subchunk, 16 if uncompressed


	fread(&sWaveHeader.formatType, sizeof(short), 1, pFile); // 1 = uncompressed


	fread(&sWaveHeader.channels, sizeof(short), 1, pFile); // mono or stereo



	fread(&sWaveHeader.sampleRate, sizeof(int), 1, pFile);

	fread(&sWaveHeader.avgBytesPerSec, sizeof(int), 1, pFile);// == SampleRate * NumChannels * BitsPerSample/8

	fread(&sWaveHeader.bytesPerSample, sizeof(short), 1, pFile);// == NumChannels * BitsPerSample/8

	fread(&sWaveHeader.bitsPerSample, sizeof(short), 1, pFile);

	fread(&sWaveHeader.subChunk2ID, sizeof(char), 4, pFile);// "data"
	if (sWaveHeader.subChunk2ID[0] != 'd' || sWaveHeader.subChunk2ID[1] != 'a' || sWaveHeader.subChunk2ID[2] != 't' || sWaveHeader.subChunk2ID[3] != 'a')
	{
		printf("subChunk2ID error");
		return;
	}


	fread(&sWaveHeader.subChunk2Size, sizeof(int), 1, pFile);// == NumSamples * NumChannels * BitsPerSample/8


	//char* buf = new char[sWaveHeader.subChunk2Size];//Both these lines work
	byte* buf = new byte[sWaveHeader.subChunk2Size];//Both these lines work
	//ALbyte* buf = new ALbyte[sWaveHeader.subChunk2Size];//Both these lines work
	//int* buf = new int[sWaveHeader.subChunk2Size];//Both these lines work

	fread(buf, sizeof(byte), sWaveHeader.subChunk2Size, pFile);



	//fread(&sWaveHeader.channels, sizeof(short), 1, pFile);
	//fwrite(inputBufferByteAddress, sizeof(byte), intermediateBuffer.size(), pFile);       // == NumSamples * NumChannels * BitsPerSample/8
	fclose(pFile);


	int sampleIndex = 0;


	intDataStore.resize(sWaveHeader.channels);
	//resize the first vector of vectors
	//thenr esize each vector that is in them

	for (int i = 0; i < sWaveHeader.channels; i++)
	{
		intDataStore[i].resize(sWaveHeader.subChunk2Size / sWaveHeader.bytesPerSample);
	}



	for (auto i = 0; i < sWaveHeader.subChunk2Size;)
	{
		for (auto j = 0; j < sWaveHeader.channels; j++)
		{
			int high = (int)buf[i];
			i++;
			int low = (int)buf[i];
			i++;
			int sample = (high >> 8) + (low & 0xff);

			intDataStore[j][sampleIndex] = sample;

		}
		sampleIndex++;
	}

	//std::ofstream datbuf0("datbuf0.txt");
	//for (auto i = 0; i < sWaveHeader.subChunk2Size; i+=2)
	//{
	//	datbuf0 << (int)buf[i] << "\n";
	//}
	//datbuf0.close();

	//std::ofstream datbuf1("datbuf1.txt");
	//for (auto i = 1; i < sWaveHeader.subChunk2Size; i += 2)
	//{
	//	datbuf1 << (int)buf[i] << "\n";
	//}
	//datbuf1.close();


	//output prep
	outputDevice = alcOpenDevice(NULL);
	if (!outputDevice)
	{
		printf("outputDevice error");
	}
	outputContext = alcCreateContext(outputDevice, NULL);
	if (!alcMakeContextCurrent(outputContext))
	{
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




	alBufferData(outputBuffer, AL_FORMAT_STEREO16, (ALvoid *)buf, sWaveHeader.subChunk2Size, FREQ);//Store the sound data in the OpenAL Buffer



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
	alSourcei(outputSource, AL_LOOPING, AL_TRUE);


	//Actually play the input
	alSourcePlay(outputSource);
	printf("subChunk2Size=%d\n", sWaveHeader.subChunk2Size);
	printf("bytesPerSample=%d\n", sWaveHeader.bytesPerSample);
	printf("bitsPerSample=%d\n", sWaveHeader.bitsPerSample);
	printf("avgBytesPerSec=%d\n", sWaveHeader.avgBytesPerSec);
	printf("sampleRate=%d\n", sWaveHeader.sampleRate);
	printf("channels=%d\n", sWaveHeader.channels);


	printf("Keys h: pause\n");
	printf("Keys p: play\n");
	printf("Keys s: stop\n");
	printf("Keys q: quit\n");

	ALubyte c = ' ';
	while (c != 'q')
	{
		alGetSourcei(outputSource, AL_SOURCE_STATE, &outputSourceState);
		if (outputSourceState == AL_PLAYING)
		{
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



}





void RecordAndSave()
{

	WAVE_Format sWaveHeader;


	//input vars
	ALCint inputSample = 0;
	ALCdevice *inputDevice;
	std::vector<ALbyte> intermediateBuffer;
	//This input buffer has to stay really big compared to the buffersize 10 seems reasonable enough
	//Might be a better variable for this
	ALubyte inputBuffer[BUFFERSIZE * 10];


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
	if (!inputDevice)
	{
		printf("inputDevice error");
	}



	//output prep
	outputDevice = alcOpenDevice(NULL);
	if (!outputDevice)
	{
		printf("outputDevice error");
	}
	outputContext = alcCreateContext(outputDevice, NULL);
	if (!alcMakeContextCurrent(outputContext))
	{
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
	while (true)
	{
		alcGetIntegerv(inputDevice, ALC_CAPTURE_SAMPLES, 1, &inputSample);
		if (inputSample > BUFFERSIZE)
		{
			alcCaptureSamples(inputDevice, inputBuffer, inputSample);

			//Input samples size for different AUDIO samples
			//Mono8 = 1Byte
			//Mono16 = 2Bytes
			//Stereo8 = 2Bytes
			//Stereo16 = 4Bytes
			//insert all data into my mega buffer 
			intermediateBuffer.insert(intermediateBuffer.end(), &inputBuffer[0], &inputBuffer[inputSample * 4]);
		}
		auto keyPressed = GetAsyncKeyState(VK_ESCAPE);
		if (keyPressed)
		{
			break;
		}
	}


	ALbyte* inputBufferByteAddress = &intermediateBuffer[0];
	alBufferData(outputBuffer, AL_FORMAT_STEREO16, (ALvoid *)inputBufferByteAddress, intermediateBuffer.size(), FREQ);//Store the sound data in the OpenAL Buffer


	//This is wave formatting
	sprintf_s(sWaveHeader.chunkID, sizeof(sWaveHeader.chunkID), "RIFF");
	sWaveHeader.chunkSize = 36 + 16;
	sprintf_s(sWaveHeader.format, sizeof(sWaveHeader.format), "WAVE");
	sprintf_s(sWaveHeader.subChunk1ID, sizeof(sWaveHeader.subChunk1ID), "fmt ");
	sWaveHeader.subChunk1Size = 16;
	sWaveHeader.formatType = WAVE_FORMAT_PCM;
	sWaveHeader.channels = 2;
	sWaveHeader.sampleRate = FREQ;
	sWaveHeader.bitsPerSample = 16;
	sWaveHeader.bytesPerSample = sWaveHeader.channels * sWaveHeader.bitsPerSample / 8;
	sWaveHeader.avgBytesPerSec = sWaveHeader.sampleRate * sWaveHeader.bytesPerSample;
	sprintf_s(sWaveHeader.subChunk2ID, sizeof(sWaveHeader.subChunk2ID), "data");
	sWaveHeader.subChunk2Size = intermediateBuffer.size();// *sWaveHeader.bytesPerSample;


	//This is wave saving
	FILE * pFile;
	fopen_s(&pFile, "myfile.wav", "wb");
	fwrite(sWaveHeader.chunkID, sizeof(char), 4, pFile);             // "RIFF"
	fwrite(&sWaveHeader.chunkSize, sizeof(int), 1, pFile);           // 4 + (8 + SubChunk1Size) + (8 + SubChunk2Size)
	fwrite(sWaveHeader.format, sizeof(char), 4, pFile);              // "WAVE"
	fwrite(sWaveHeader.subChunk1ID, sizeof(char), 4, pFile);         // "fmt "
	fwrite(&sWaveHeader.subChunk1Size, sizeof(int), 1, pFile);       // bytes remaining in subchunk, 16 if uncompressed
	fwrite(&sWaveHeader.formatType, sizeof(short), 1, pFile);       // 1 = uncompressed
	fwrite(&sWaveHeader.channels, sizeof(short), 1, pFile);       // mono or stereo
	fwrite(&sWaveHeader.sampleRate, sizeof(int), 1, pFile);
	fwrite(&sWaveHeader.avgBytesPerSec, sizeof(int), 1, pFile);            // == SampleRate * NumChannels * BitsPerSample/8
	fwrite(&sWaveHeader.bytesPerSample, sizeof(short), 1, pFile);        // == NumChannels * BitsPerSample/8
	fwrite(&sWaveHeader.bitsPerSample, sizeof(short), 1, pFile);
	fwrite(sWaveHeader.subChunk2ID, sizeof(char), 4, pFile);         // "data"
	fwrite(&sWaveHeader.subChunk2Size, sizeof(int), 1, pFile);       // == NumSamples * NumChannels * BitsPerSample/8
	fwrite(inputBufferByteAddress, sizeof(byte), intermediateBuffer.size(), pFile);       // == NumSamples * NumChannels * BitsPerSample/8
	fclose(pFile);



	//grab the first address of buffer
	//might be a neater way of doing this

	auto res = alGetError();
	if (alGetError() != AL_NO_ERROR)
	{
		printf("loading al buffer error");
		system("PAUSE");
		return;
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
		if (outputSourceState == AL_PLAYING)
		{
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

}