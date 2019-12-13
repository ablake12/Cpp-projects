#include "rnb.h"


//Dj Controller
//Alanza Blake
//Hawa Wague
//Stephanie Obwar


Sample samp1 = Sample("Verse1.mp3", 0, true); //The second argument indicates the channel - only one sample can be played per channel - you can have a max of 32 channels
Sample samp2 = Sample("Verse2.mp3", 0, true); //the third argument indicates that the sample should play in loop mode where it loops until you stop the program
Sample samp3 = Sample("Verse3.mp3", 0, true); //Put the full file path to your sound for the first argument - for now it only plays wav files
Sample samp4 = Sample("Verse.mp3", 0, true);
Sample playing1 = samp1;//initialize a sample variable called playing to control the song currently playing

float lcv1 = 0.0;//initialize last channel volume for left side

Sample samp5 = Sample("Verse5.mp3", 1, true);
Sample samp6 = Sample("Verse6.mp3", 1, true);
Sample samp7 = Sample("Verse7.mp3", 1, true);
Sample samp8 = Sample("Verse8.mp3", 1, true);
Sample playing2 = samp5;//initialize a sample variable for current song playing in that channel

float lcv2 = 0.0;//initialize last channel volume for right side

int cross = 0;//initialize variable to store where the cross fader is currently at

void midi_data_in(double deltatime, std::vector< unsigned char > *message, void */*userData*/)
{

	unsigned int nBytes = message->size();
	//for (unsigned int i = 0; i < nBytes; i++)
		//cout << "Byte " << i << " = " << (int)message->at(i) << ", ";
	//if (nBytes > 0)
		//cout << "stamp = " << deltatime << std::endl;


	//Cross fader

	//store cross fader switch into variables so we can adjust the volume correctly
	if (message->at(0) == 176 && message->at(1) == 7 && message->at(2) == 0)//if it's on the left (1)
	{
		cross = 1;//store 1 if it's on the left
	}

	else if (message->at(0) == 176 && message->at(1) == 7 && message->at(2) == 127)//if it's on the right (2)
	{
		cross = 2;//store 2 if it's on the right
	}
	else if (message->at(0) == 176 && message->at(1) == 7 && message->at(2) == 64)//if it's in the middle
	{
		cross = 3;//store 3 if it's in the middle
	}

	//Now set volume based of where the cross fader lies
	if (cross == 1)//cross fader on the left
	{
		playing1.set_volume(lcv1 * 1.0);//playing at regular vol
		playing2.set_volume(lcv2 * 0.0);//playing at 0
	}

	else if (cross == 3)//cross fader on the middle
	{
		playing1.set_volume(lcv1 * 0.5);//playing at half vol
		playing2.set_volume(lcv2 * 0.5);//playing at half vol
	}

	else if (cross == 2)//cross fader on the right
	{
		playing1.set_volume(lcv1 * 0.0);//playing at 0
		playing2.set_volume(lcv2 * 1.0);//playing at regular vol
	}

	//Store last channel volume into variables

	if (message->at(0) == 177 && message->at(1) == 5)//volume controls on the left
	{
		lcv1 = message->at(2) / 127.0;//ranges from 0-1
	}

	if (message->at(0) == 178 && message->at(1) == 5)//volume control right
	{
		lcv2 = message->at(2) / 127.0;//ranges from 0-1
	}


	//left turntables - determine whether the s[ong is played forward or backwards
	if (message->at(0) == 177 && message->at(1) == 17 && message->at(2) == 1)//bytes equal to left turntable going forwards
	{
		playing1.set_speed(1);//set speed to this
	}
	else if(message->at(0) == 177 && message->at(1) == 17 && message->at(2) == 127)//bytes equal to left turntable going backwards
	{
		playing1.set_speed(-1);//set speed to this
	}

	//Left speed slider
	if (message->at(0) == 177 && message->at(1) == 24)//bytes equal to left speed slider
	{
		if (message->at(2) >= 0 && message->at(2) <= 64)//byte 2 is between 0-64
		{
			playing1.set_speed((message->at(2)/128.0) + 0.5);//set speed to this
		}
		else if (message->at(2) >= 65 && message->at(2) <= 127)//byte 2 is between 65-127
		{
			playing1.set_speed(((message->at(2) - 65.0)/124.0) + 1.0);//set speed to this
		}
	}


	//Bass control for left side
	if (message->at(0) == 177 && message->at(1) == 3)//if bytes equal the amount of bytes for the left bass control
	{
		playing1.add_bass_mid_treble_ctrl();//to control bass control
		if (message->at(2) >= 0 && message->at(2) <= 64)//byte 2 is 0-64
		{
			playing1.set_bass((message->at(2) * 1.25) - 80.0);//set bass to this
		}
		else if (message->at(2) >= 65 && message->at(2) <= 127)//byte 2 is 65-127
		{
			playing1.set_bass((message->at(2) - 65) / 6.2);//set bass to this
		}
		
	}
	//Mid control for left side
	if (message->at(0) == 177 && message->at(1) == 2)//bytes equal to left mid control bytes
	{
		playing1.add_bass_mid_treble_ctrl();//to control mid controls
		if (message->at(2) >= 0 && message->at(2) <= 64)//byte 2 is between 0-64
		{
			playing1.set_mids((message->at(2) * 1.25) - 80.0);//set mid to this
		}
		else if (message->at(2) >= 65 && message->at(2) <= 127)//byte 2 is between 65-127
		{
			playing1.set_mids((message->at(2) - 65) / 6.2);//set mid to this
		}

	}

	//Treble control for left side
	if (message->at(0) == 177 && message->at(1) == 1)//bytes equal to left treble control bytes
	{
		playing1.add_bass_mid_treble_ctrl();//to control bass controls
		if (message->at(2) >= 0 && message->at(2) <= 64)//if byte 2 is between 0-64
		{
			playing1.set_highs((message->at(2) * 1.25) - 80.0);//set high to this
		}
		else if (message->at(2) >= 65 && message->at(2) <= 127)//if byte 2 is between 65-127
		{
			playing1.set_highs((message->at(2) - 65) / 6.2);//set high to this
		}

	}

	//right turntables - determine whether the song is played forward or backwards
	if (message->at(0) == 178 && message->at(1) == 17 && message->at(2) == 1)//bytes equal to the right turntable bytes
	{
		playing2.set_speed(1);//set speed if it's playing forwards
	}
	else if (message->at(0) == 178 && message->at(1) == 17 && message->at(2) == 127)//bytes equal to the turntable bytes
	{
		playing2.set_speed(-1);//set speed if it's playing backwards
	}

	//Right speed slider
	if (message->at(0) == 178 && message->at(1) == 24)//bytes equal to the right speed slider control
	{
		if (message->at(2) >= 0 && message->at(2) <= 64)//if byte 2 is between 0-64
		{
			playing2.set_speed((message->at(2) / 128.0) + 0.5);//set speed to this
		}
		else if (message->at(2) >= 65 && message->at(2) <= 127)//if byte 2 is between 65-127
		{
			playing2.set_speed(((message->at(2) - 65.0) / 124.0) + 1.0);//set speed to this
		}
	}

	//Bass control for right side

	if (message->at(0) == 178 && message->at(1) == 3)//if bytes equal the amount of bytes for the right bass control
	{
		playing2.add_bass_mid_treble_ctrl();//to control bass level
		if (message->at(2) >= 0 && message->at(2) <= 64)//if byte 2 is 0-64
		{
			playing2.set_bass((message->at(2) * 1.25) - 80.0);//set bass to this
		}
		else if (message->at(2) >= 65 && message->at(2) <= 127)//if byte 2 is 65-127
		{
			playing2.set_bass((message->at(2) - 65) / 6.2);//set bass to this
		}

	}

	//Mid control for right side

	if (message->at(0) == 178 && message->at(1) == 2)//bytes for right mid control
	{
		playing2.add_bass_mid_treble_ctrl();//to control mid level
		if (message->at(2) >= 0 && message->at(2) <= 64)//if byte 2 is 0-64
		{
			playing2.set_mids((message->at(2) * 1.25) - 80.0);//set mid to this
		}
		else if (message->at(2) >= 65 && message->at(2) <= 127)//if byte 2 is 65-127
		{
			playing2.set_mids((message->at(2) - 65) / 6.2);//set mid to this
		}

	}

	//Treble control for right side
	if (message->at(0) == 178 && message->at(1) == 1)//bytes for right treble control
	{
		playing2.add_bass_mid_treble_ctrl();//to control the treble levels
		if (message->at(2) >= 0 && message->at(2) <= 64)//if byte 2 is between 0-64
		{
			playing2.set_highs((message->at(2) * 1.25) - 80.0);//set highs to this
		}
		else if (message->at(2) >= 65 && message->at(2) <= 127)//if byte 2 is between 65-127
		{
			playing2.set_highs((message->at(2) - 65) / 6.2);//set highs to this
		}

	}

	//left song pads
	if (message->at(0) == 145 && message->at(1) == 27 && message->at(2) == 0)//samp1
	{
		samp2.set_paused(true);//pause any other sample so it won't play at the same time
		samp3.set_paused(true);//pause any other sample so it won't play at the same time
		samp4.set_paused(true);//pause any other sample so it won't play at the same time
		playing1 = samp1;//set the current song to this sample
		samp1.play();

	}
	else if (message->at(0) == 145 && message->at(1) == 28 && message->at(2) == 0)//samp2
	{
		samp1.set_paused(true);//pause any other sample so it won't play at the same time
		samp3.set_paused(true);//pause any other sample so it won't play at the same time
		samp4.set_paused(true);//pause any other sample so it won't play at the same time
		playing1 = samp2;//set the current song to this sample
		samp2.play();//play sample
	}
	else if (message->at(0) == 145 && message->at(1) == 29 && message->at(2) == 0)//samp3
	{
		samp1.set_paused(true);//pause any other sample so it won't play at the same time
		samp2.set_paused(true);//pause any other sample so it won't play at the same time
		samp4.set_paused(true);//pause any other sample so it won't play at the same time
		playing1 = samp3;//set the current song to this sample
		samp3.play();//play sample
	}
	else if (message->at(0) == 145 && message->at(1) == 30 && message->at(2) == 0)//samp4
	{
		samp1.set_paused(true);//pause any other sample so it won't play at the same time
		samp2.set_paused(true);//pause any other sample so it won't play at the same time
		samp3.set_paused(true);//pause any other sample so it won't play at the same time
		playing1 = samp4;//set the current song to this sample
		samp4.play();//play sample
	}
	
	//right song pads
	if (message->at(0) == 146 && message->at(1) == 27)//samp 5
	{
		samp6.set_paused(true);//pause if playing
		samp7.set_paused(true);//pause if playing
		samp8.set_paused(true);//pause if playing
		playing2 = samp5;//set song to the currently song playing
		samp5.play();
	}
	else if (message->at(0) == 146 && message->at(1) == 28)//samp6
	{
		samp5.set_paused(true);//pause if playing
		samp7.set_paused(true);//pause if playing
		samp8.set_paused(true);//pause if playing
		playing2 = samp6;//set song to the currently song playing
		samp6.play();//play song
	}
	else if (message->at(0) == 146 && message->at(1) == 29)//samp7
	{
		samp5.set_paused(true);//pause if playing
		samp6.set_paused(true);//pause if playing
		samp8.set_paused(true);//pause if playing
		playing2 = samp7;//set song to the currently song playing
		samp7.play();//play song
	}
	else if (message->at(0) == 146 && message->at(1) == 30)//samp8
	{
		samp5.set_paused(true);//pause if playing
		samp6.set_paused(true);//pause if playing
		samp7.set_paused(true);//pause if playing
		playing2 = samp8;//set song to the currently song playing
		samp8.play();//play song
	}
}
int main()
{
	midIn djController = midIn(midi_data_in);//create set midIn variable
	while (true)
	{
		samp1.upd();//update the audio engine to apply any changes to the sound
		Sleep(50);//helps the audio play smoothly
	}
}
