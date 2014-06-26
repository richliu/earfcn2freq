




#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>

typedef struct EUTRA_FREQ {
	int band; 
	float Ful_low, Ful_high, Fdl_low, Fdl_high;
	float Foffs_ul, Foffs_dl; /* In spec, opposit */
}EUTRA_FREQ ;	

#define NELEMS(x)  (sizeof(x) / sizeof(x[0]))

EUTRA_FREQ eutra_freq[] = {
	{ 1, 1920, 1980, 2110, 2170, 18000, 0}, 
	{ 2, 1850, 1910, 1930, 1990, 18600, 600},
	{ 3, 1710, 1785, 1805, 1880, 19200, 1200},
	{ 4, 1710, 1755, 2110, 2155, 19950, 1950},
	{ 5,  824,  849,  869,  894, 20400, 2400},
	{ 6,  830,  840,  875,  885, 20650, 2650}, 
	{ 7, 2500, 2570, 2620, 2690, 20750, 2750}, 
	{ 8,  880,  915,  925,  960, 21450, 3450}, 
	{ 9, 1749.9, 1784.9, 1844.9, 1879.9 , 21800, 3800}, 
	{10, 1710, 1770, 2110, 2170, 22150,4150 },
	{11, 1427.0, 1447.9, 1475.9, 1495.9, 22750,4750},
	{12, 699, 716, 729, 746, 23010, 5010},
	{13, 777, 787, 746, 756, 23180, 5180},
	{14, 788, 798, 758, 768, 23280,5280},
	{17, 704, 716, 734, 746, 23730,5730},
	{18, 815, 830, 860, 875, 23850,5850},
	{19, 830, 845, 875, 890, 24000,6000},
	{20, 832, 862, 791, 821, 24150,6150},
	{21, 1447.9, 1462.9, 1495.9, 1510.9, 24450,6450},
	{22, 3410, 3490, 3510, 3590, 24600,6600},
	{23, 2000, 2020, 2180, 2200, 25500,7500},
	{24, 1626.5, 1660.5, 1525, 1559, 25700,7700},
	{25, 1850, 1915, 1930, 1995, 26040,8040},
	{26, 814, 849, 859, 894, 26690,8690},
	{27, 807, 824, 852, 869, 27040,9040},
	{28, 703, 748, 758, 803, 27210,9210},
	{29, 0,   0,   717, 728, 0, 9660},
	{30, 2305, 2315, 2350, 2360, 27660,9770},
	{31, 452.5, 457.5, 462.5, 467.5, 27760,9870},
	{33, 1900, 1920, 1900, 1920, 36000, 36000},
	{34, 2010, 2025, 2010, 2025, 36200, 36200},
	{35, 1850, 1910, 1850, 1910, 36350, 36350}, 
	{36, 1930, 1990, 1930, 1990, 36950, 36950},
	{37, 1910, 1930, 1910, 1930, 37550, 37550},
	{38, 2570, 2620, 2570, 2620, 37750, 37750},
	{39, 1880, 1920, 1880, 1920, 38250, 38250},
	{40, 2300, 2400, 2300, 2400, 38650, 38650},	
	{41, 2496, 2690, 2496, 2690, 39650, 39650},
	{42, 3400, 2600, 2400, 3600, 41590, 41590},
	{43, 3600, 3800, 3600, 3800, 43590, 43590},
	{44,  703, 803, 703, 803, 45590, 45590},
	{ 0, 0 , 0 , 0 , 0, 0, 0}
};


enum OPTION_TYPE { otDEFAULT, otBAND, otFREQ, otEARFCN, otDUMP };
enum OPTION_TYPE option_type = otDEFAULT;

int eutra_freq_size = NELEMS(eutra_freq);
float freq;
int band=-1;
int earfcn=-1;


int freq2earfcn(int band, float freq){
	int i;
	int dl=0;

	for(i=0 ; i < eutra_freq_size ; i++){
		if ( band == eutra_freq[i].band ){
			if( freq >= eutra_freq[i].Ful_low && freq <= eutra_freq[i].Ful_high){
				/* UL */
				earfcn = (freq-eutra_freq[i].Ful_low)*10 + eutra_freq[i].Foffs_ul;
			}else if( freq >= eutra_freq[i].Fdl_low && freq <= eutra_freq[i].Fdl_high ){
				/* DL */
				earfcn = (freq-eutra_freq[i].Fdl_low)*10 + eutra_freq[i].Foffs_dl;
				dl = 1;
			}else{
				printf("Freq out of band spec, input freq: %.1f \n", freq);
				printf("Band %d \n", band);
				printf("UL Freq: %.1f-%.1f\n", eutra_freq[i].Ful_low, eutra_freq[i].Ful_high);
				printf("DL Freq: %.1f-%.1f\n", eutra_freq[i].Fdl_low, eutra_freq[i].Fdl_high);
			}
		}
	}

	if( earfcn != -1){
		if ( option_type == otEARFCN ) {
			printf("%d",earfcn);
		}else{
			printf("%s EARFCN: %d \n",dl==1?"DL":"UL", earfcn);
		}
	}else{
		printf(" Band %d is not exist \n", band);
	}

	return 0;
}

float earfcn2freq(int earfcn){
	int i=0;
	float Fdl=-1;
	int dl=0;

	if (earfcn < 18000) {
		/* DL */
		for(i=0; i< eutra_freq_size; i++){
			if( earfcn >= eutra_freq[i].Foffs_dl && 
			    earfcn < (eutra_freq[i].Foffs_dl + (eutra_freq[i].Fdl_high-eutra_freq[i].Fdl_low)*10) ){
				Fdl = eutra_freq[i].Fdl_low + 0.1*(earfcn - eutra_freq[i].Foffs_dl);
				band = eutra_freq[i].band;
				dl = 1;
				break;
			}
		}


	}else if (earfcn >=18000 && earfcn <= 65535){
		/* UL */
		for(i=0; i< eutra_freq_size; i++){
			if( earfcn >= eutra_freq[i].Foffs_ul && 
				earfcn < (eutra_freq[i].Foffs_ul + (eutra_freq[i].Ful_high-eutra_freq[i].Ful_low)*10)){
				Fdl = eutra_freq[i].Ful_low + 0.1*(earfcn - eutra_freq[i].Foffs_ul);
				band = eutra_freq[i].band;
				break;
			}
		}


	}else {
		printf("Wrong earfcn %d \n", earfcn);
		return -1;
	}

	switch( option_type ){
		case otBAND:
			printf("%d\n", band);
			break;

		case otFREQ:
			printf("%.1f\n", Fdl);
			break;

		default:
			printf("%s EARFCN: %d \n",dl==1?"DL":"UL", earfcn);
			printf("FREQ : %0.1f \n", Fdl);
			printf("BAND : %d \n", band);
			break;
	}
	return Fdl;

}


int main(int argc, char *argv[]){
	float input_number;
	int c;

    while ((c = getopt (argc, argv, "bfedB:")) != -1){
        switch (c){
            case 'B':
				sscanf(optarg, "%d", &band);
                break;
            case 'b':
                option_type = otBAND;
                break;
            case 'f':
                option_type = otFREQ;
                break;
            case 'e':
                option_type = otEARFCN;
                break;
            case 'd':
                option_type = otDUMP;
                break;
            case '?':
                fprintf (stderr, "Unknown option character `\\x%x'.\n", optopt);
                return 1;
            default:
                return -1;
        }
    }

	if ( NULL == argv[optind] ){
		printf("usage : earfcn2freq [option] <earfcn or freq> \n");
		printf(" option -B <band> : input number is freq, assign Band\n");
		printf(" option -b : output band only(input is earfcn\n");
		printf(" option -f : output freq only(input is earfcn)\n");
		printf(" option -e : output earfcn only(input is freq)\n");
		return -1;
	}

	sscanf( argv[optind], "%f", &input_number);

	if( band == -1 ){
		earfcn2freq(input_number);
	}else{
		freq2earfcn( band, input_number);
	}

	return 0;
}


