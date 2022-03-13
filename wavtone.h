#ifndef WAV_GEN_H
#define WAV_GEN_H

#include <stdint.h>
#include <stdio.h>
#include <math.h>

// ID constants in little endian
#define L_CHUNK_ID		0x46464952	// "FFIR" (or "RIFF")
#define L_FORMAT		0x45564157	// "EVAW" (or "WAVE")
#define L_SB1_ID		0x20746D66	// " tmf" (or "ftm ")
#define L_SB2_ID		0x61746164	// "atad" (or "data")

// ID constants in big endian (how they should be written as)
#define CHUNK_ID		0x52494646	// RIFF
#define FORMAT			0x57415645	// "EVAW" (or "WAVE")
#define SB1_ID			0x666D7420	// " tmf" (or "ftm ")
#define SB2_ID			0x64617461	// "atad" (or "data")

#define PCM				0x01		// PCM format indicator
#define PCM_SB1_SIZE	16			

typedef struct pcm_wav_header {
	// RIFF chunk descriptor
	uint32_t chunk_id;			// "RIFF" in ASCII (big-endian)
	uint32_t chunk_size;		// Size of remainder of file in bytes
	uint32_t format;			// "WAVE" in ASCII (big-endian)

	// fmt sub-chunk
	uint32_t subchunk_1_id;		// "fmt " in ASCII (big-endian)
	uint32_t subchunk_1_size;	// Size of this subchunk in bytes (16 for PCM)
	uint16_t audio_format;		// Format of which audio is stored. (0x01 for PCM)
	uint16_t num_channels;		// 1 = Mono, 2 = Stereo
	uint32_t sample_rate;		// Number of samples played back per second
	uint32_t byte_rate;			// sample_rate * num_channels * (bits_per_sample/8)
	uint16_t block_align;		// num_channels * (bits_per_sample/8)
	uint16_t bits_per_sample;	// Bit depth

	// data sub-chunk
	uint32_t subchunk_2_id;		// "data" in ASCII (big-endian)
	uint32_t subchunk_2_size;	// Number of bytes of data following this header

	// Remainder of the file is sound data
} pcm_wav_header;

void print_pcm_wave_header(struct pcm_wav_header* hdr){
	printf("Estimated file size: %d bytes\n\n", hdr->chunk_size + 8);
	printf("chunk_id:            0x%08X\n", hdr->chunk_id);
	printf("chunk_size:          %d\n", hdr->chunk_size);
	printf("format:              0x%08X\n", hdr->format);
	printf("subchunk_1_id:       0x%08X\n", hdr->subchunk_1_id);
	printf("subchunk_1_size:     %d\n", hdr->subchunk_1_size);
	printf("audio_format:        %d\n", hdr->audio_format);
	printf("num_channels:        %d\n", hdr->num_channels);
	printf("sample_rate:         %d\n", hdr->sample_rate);
	printf("byte_rate:           %d\n", hdr->byte_rate);
	printf("block_align:         %d\n", hdr->block_align);
	printf("bits_per_sample:     %d\n", hdr->bits_per_sample);
	printf("subchunk_2_id:       0x%08X\n", hdr->subchunk_2_id);
	printf("subchunk_2_size:     %d\n", hdr->subchunk_2_size);
}

pcm_wav_header gen_pcm_wav_header(
	uint16_t channels, uint32_t rate, uint16_t depth
){
	struct pcm_wav_header hdr;

	hdr.chunk_id = L_CHUNK_ID;
	hdr.format = L_FORMAT;
	hdr.subchunk_1_id = L_SB1_ID;
	hdr.subchunk_1_size = PCM_SB1_SIZE;
	hdr.audio_format = PCM;
	hdr.subchunk_2_id = L_SB2_ID;
	hdr.subchunk_2_size = 0;

	hdr.num_channels = channels;
	hdr.sample_rate = rate;
	hdr.bits_per_sample = depth;

	hdr.block_align = hdr.num_channels * (hdr.bits_per_sample / 8);
	hdr.byte_rate = hdr.sample_rate * hdr.block_align;
	hdr.chunk_size = sizeof(hdr) - 8;
	
	return hdr;
}

void update_pcm_wav_size(pcm_wav_header *hdr, uint32_t size){
	hdr->subchunk_2_size += size;
	hdr->chunk_size += size;
}

void generate_sine(FILE *fp, pcm_wav_header *hdr, double sec, uint32_t freq){
	printf("Generating %u Hz sine wave for %f seconds\n", freq, sec);
	uint32_t num_samples = (uint32_t)(sec * (double)(hdr->sample_rate));
	double dx = ((double)(freq) / (hdr->sample_rate)) * M_PI;
	uint32_t i = 0;
	for(; i < num_samples; i++){
		int16_t sample = (int16_t)(32767*sin(dx*i));
		fwrite(&sample, sizeof(sample), 1, fp);
	}
	update_pcm_wav_size(hdr, num_samples);
}

int demo(){
	FILE *fp = fopen("test.wav", "r");

	if(fp){
		printf("Error: file exists\n");
		return -1;
	}

	fp = fopen("test.wav", "w");
	pcm_wav_header hdr = gen_pcm_wav_header(1, 44100, 16);
	fwrite(&hdr, sizeof(hdr), 1, fp);
	generate_sine(fp, &hdr, 5.0, 440);
	printf("Demo file created as test.wav\n");
	print_pcm_wave_header(&hdr);
	fclose(fp);

	return 0;
}

#endif
