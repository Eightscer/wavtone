#include "wavtone.h"

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

void generate_sine(
	FILE *fp, pcm_wav_header *hdr, double sec, uint32_t freq
){
	printf("Generating %u Hz sine wave for %f seconds\n", freq, sec);
	uint32_t num_samples = (uint32_t)(sec * (double)(hdr->sample_rate));
	double dx = (((double)(freq) / (hdr->sample_rate)) * M_PI) * 2;
	uint32_t i = 0;
	int16_t sample = 0;
	for(; i < num_samples; i++){
		sample = (int16_t)(32767*sin(dx*i));
		fwrite(&sample, sizeof(sample), 1, fp);
	}
	update_pcm_wav_size(hdr, num_samples);
}

void generate_square(
	FILE *fp, pcm_wav_header *hdr, double sec, uint32_t freq, double duty
){
	printf("Generating %u Hz square wave with %f duty cycle for %f seconds\n",
		freq, duty, sec);
	uint32_t num_samples = (uint32_t)(sec * (double)(hdr->sample_rate));
	double max = (double)(hdr->sample_rate) / (double)(freq);
	double high = max * duty;
	double x = 0.0;
	uint32_t i = 0;
	int16_t sample = 0;
	for(; i < num_samples; i++){
		if(x < high)
			sample = -32767;
		else
			sample = 32767;

		if(x > max)
			x = 0;
		else
			x++;
			
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
	//generate_square(fp, &hdr, 5.0, 440, 0.5);
	printf("Demo file created as test.wav\n");
	print_pcm_wave_header(&hdr);
	fclose(fp);

	return 0;
}
