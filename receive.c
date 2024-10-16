#include <types.c>
#include <WinAPI/msvcrt.c>


//#define FREQUENCY   2000
//#define WAVE_LENGTH (22050 / FREQUENCY)
//#define WAVE_LENGTH (4)
//#define WAVE_LENGTH (3)
//#define WAVE_LENGTH (15)
//#define WAVE_LENGTH (16)
#define WAVE_LENGTH (32)


Signed_Number16 buffer[WAVE_LENGTH];


void receive_ASK()
{
	Number i;
	Number j;
	Byte   byte;

	for(j = 0; j < WAVE_LENGTH; ++j) {
		if(read_bytes(&stdin, buffer + j, sizeof(buffer[j])) != sizeof(buffer[j])) {
			return;
		}

		if(read_bytes(&stdin, buffer + j, sizeof(buffer[j])) != sizeof(buffer[j])) {
			return;
		}
	}

	for(;;) {
		byte = 0;

		for(i = 0; i < 8;) {
			Signed_Number max_zero;
			Signed_Number max_one;
			Signed_Number min_zero;
			Signed_Number min_one;

			max_zero = 0;
			max_one = 0;
			min_zero = 0;
			min_one = 0;

			for(j = 1; j < WAVE_LENGTH - 1; ++j) {
				Signed_Number amplitude = buffer[j];

				if(
					amplitude >= buffer[j - 1]
					//&& amplitude > buffer[j - 2]
					&& amplitude >= buffer[j + 1]
					//&& amplitude > buffer[j + 2]
				) {
					if(amplitude > 32768 / 4) {
						++max_one;
					}
					else if(amplitude > 32768 / 16) {
						++max_zero;
					}
				}

				if(
					amplitude <= buffer[j - 1]
					//&& amplitude < buffer[j - 2]
					&& amplitude <= buffer[j + 1]
					//&& amplitude < buffer[j + 2]
				) {
					if(amplitude < -(32768 / 4)) {
						++min_one;
					}
					else if(amplitude < -(32768 / 16)) {
						++min_zero;
					}
				}
			}

			if(max_zero == 2 && min_zero == 2 || max_one == 2 && min_one == 2) {
				if(max_zero == 2 && min_zero == 2) {
					byte &= ~(1 << i);
				}
				else {
					byte |= (1 << i);
				}

				++i;


				for(j = 0; j < WAVE_LENGTH; ++j) {
					if(read_bytes(&stdin, buffer + j, sizeof(buffer[j])) != sizeof(buffer[j])) {
						return;
					}

					if(read_bytes(&stdin, buffer + j, sizeof(buffer[j])) != sizeof(buffer[j])) {
						return;
					}
				}
			}
			else {
				for(j = 0; j < WAVE_LENGTH - 1; ++j) {
					buffer[j] = buffer[j + 1];
				}

				if(read_bytes(&stdin, &buffer[WAVE_LENGTH - 1], sizeof(buffer[WAVE_LENGTH - 1])) != sizeof(buffer[WAVE_LENGTH - 1])) {
					return;
				}

				if(read_bytes(&stdin, &buffer[WAVE_LENGTH - 1], sizeof(buffer[WAVE_LENGTH - 1])) != sizeof(buffer[WAVE_LENGTH - 1])) {
					return;
				}
			}
		}

		write_bytes(&stdout, &byte, sizeof(byte));
	}
}


Number main()
{
	receive_ASK();

	return 0;
}