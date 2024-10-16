#include <types.c>
#include <WinAPI/msvcrt.c>


//#define FREQUENCY   2000
//#define WAVE_LENGTH (22050 / FREQUENCY)
//#define WAVE_LENGTH (4)
//#define WAVE_LENGTH (3)
//#define WAVE_LENGTH (15)
//#define WAVE_LENGTH (16)
#define WAVE_LENGTH (32)


Signed_Number16 zero[WAVE_LENGTH];
Signed_Number16 one[WAVE_LENGTH];


Signed_Number16 stereo_zero[WAVE_LENGTH * 2];
Signed_Number16 stereo_one[WAVE_LENGTH * 2];
Signed_Number16 stereo_no_signal[WAVE_LENGTH * 2] = {0};


void send_ASK()
{
	Number i;
	Number j;

	for(i = 0; i < WAVE_LENGTH; ++i) {
		zero[i] = 16384 * sinf(2.0f * 2.0f * PI * ((float)i / (float)WAVE_LENGTH));
		one[i] = 32767 * sinf(2.0f * 2.0f * PI * ((float)i / (float)WAVE_LENGTH));
	}


	for(i = 0; i < WAVE_LENGTH; ++i) {
		stereo_zero[i * 2] = zero[i];
		stereo_zero[i * 2 + 1] = zero[i];
		stereo_one[i * 2] = one[i];
		stereo_one[i * 2 + 1] = one[i];
	}


	Byte             byte;
	Signed_Number16* wave;

	for(;;) {
		if(read_bytes(&stdin, &byte, sizeof(byte)) != sizeof(byte)) {
			break;
		}

		for(i = 0; i < 8; ++i) {
			if(byte & (1 << i)) {
				wave = stereo_one;//one;
			}
			else {
				wave = stereo_zero;//zero;
			}

			write_bytes(&stdout, wave, WAVE_LENGTH * 2 * sizeof(wave[j]));
			write_bytes(&stdout, stereo_no_signal, sizeof(stereo_no_signal));
		}
	}
}


Number main()
{
	send_ASK();

	return 0;
}