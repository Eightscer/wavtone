#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "wavtone.h"

void print_usage(char **argv);
int parse_args(int argc, char **argv);

double seconds = 5;
int wave = 0;
uint32_t frequency = 440;
int make_demo = 0;

int main(int argc, char **argv){
	if(argc < 2){
		print_usage(argv);
		return 1;
	}

	if(parse_args(argc, argv)){
		return 1;
	}
	
	if(make_demo)
		return demo();

	FILE *fp = fopen("output.wav", "r");

	if(fp){
		printf("Error: file exists\n");
		return 1;
	}

	fp = fopen("output.wav", "w");
	pcm_wav_header hdr = gen_pcm_wav_header(1, 44100, 16);
	fwrite(&hdr, sizeof(hdr), 1, fp);

	switch(wave){
		case SINE:
			generate_sine(fp, &hdr, seconds, frequency);
			break;
		case SQUARE:
			generate_square(fp, &hdr, seconds, frequency, 0.5);
			break;
		case SAWTOOTH:
			generate_sawtooth(fp, &hdr, seconds, frequency);
			break;
		case TRIANGLE:
			generate_triangle(fp, &hdr, seconds, frequency);
		default:
			return 1;
	}

	fclose(fp);
	printf("Output file generated. Exiting...\n");

	return 0;
}

void print_usage(char **argv){
	printf("Usage: %s [-dftw]\n", argv[0]);
	printf("-d --demo            : Outputs 440 Hz sine wave for 5 seconds\n");
	printf("-f --frequency [f]   : frequency of output tone in f Hz\n");
	printf("-t --time [s]        : duration of tone for t seconds\n");
	printf("-w --waveform [type] : waveform of output tone\n");
}

int parse_args(int argc, char **argv){
	int i = 1;
	for(; i < argc; i++){

		if(!strcmp(argv[i], "-d") || !strcmp(argv[i], "--demo")){
			make_demo = 1;
			return 0;
		}
		else if(!strcmp(argv[i], "-f") || !strcmp(argv[i], "--frequency")){
			if(i + 1 >= argc){
				printf("Error: please specify the frequency in Hz.\n");
				return 1;
			}
			frequency = (uint32_t)atoi(argv[++i]);
			if(frequency == 0){
				printf("Error: invalid frequency value.\n");
				return 1;
			}
		}

		else if(!strcmp(argv[i], "-t") || !strcmp(argv[i], "--time")){
			if(i + 1 >= argc){
				printf("Error: please specify the duration in seconds.\n");
				return 1;
			}
			seconds = atof(argv[++i]);
			if(seconds == 0.0){
				printf("Error: invalid duration value.\n");
				return 1;
			}
		}

		else if(!strcmp(argv[i], "-w") || !strcmp(argv[i], "--waveform")){
			if(i + 1 >= argc){
				printf("Error: no waveform specified.\n");
				return 1;
			}
			if(!strcmp(argv[i+1], "sine")){
				wave = SINE;
				i++;
			}
			else if(!strcmp(argv[i+1], "square")){
				wave = SQUARE;
				i++;
			}
			else if(!strcmp(argv[i+1], "sawtooth")){
				wave = SAWTOOTH;
				i++;
			}
			else if(!strcmp(argv[i+1], "triangle")){
				wave = TRIANGLE;
				i++;
			}
			else{
				printf("Error: invalid waveform argument (lowercase only).\n");
				return 1;
			}
		}

		else{
			printf("Invalid argument.\n");
			print_usage(argv);
			return 1;
		}
		
	}
	return 0;
}