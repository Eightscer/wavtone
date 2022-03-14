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

void print_pcm_wave_header(
	struct pcm_wav_header* hdr
);

pcm_wav_header gen_pcm_wav_header(
	uint16_t channels, uint32_t rate, uint16_t depth
);

void update_pcm_wav_size(
	pcm_wav_header *hdr, uint32_t size
);

void generate_sine(
	FILE *fp, pcm_wav_header *hdr, double sec, uint32_t freq
);

void generate_square(
	FILE *fp, pcm_wav_header *hdr, double sec, uint32_t freq, double duty
);

void generate_sawtooth(
	FILE *fp, pcm_wav_header *hdr, double sec, uint32_t freq
);

int demo();

#endif
