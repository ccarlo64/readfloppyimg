#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct  {
	unsigned char info[4];
	unsigned char block[35][4];
	unsigned char label[16];
	unsigned char f1[2];
	unsigned char did[2];
	unsigned char f2[1];
	unsigned char dosver[2];
	unsigned char f3[4];
	unsigned char unused[85];
} BAM;

typedef struct {
	unsigned char next_track;
	unsigned char next_sector;
	unsigned char file_type;
	unsigned char first_track;
	unsigned char first_sector;
	unsigned char file_name[16];	
	unsigned char unused[3];	
	unsigned char reserved[6];	
	unsigned char num_block[2];	
} voice;

typedef struct {
	voice e[8];
} DIR;


typedef struct {
	unsigned char track1_17[256*21*17];//5376
	BAM bam;// track 18 bam one sector +
	DIR dir[18];//256*18
	unsigned char track19_24[256*19*6];//4864
	unsigned char track25_30[256*18*6];//4608
	unsigned char track31_35[256*17*5];//4352
} disco;

void print_voice(voice dir, int idx);
void dump(unsigned char *b);
void tobinary( unsigned char byte, char *r );
void decode_type( unsigned char x );
int idxdir=0;
int savet=0,saves=0;
/*****************************

MAIN

******************************/

int main(int argc, char *argv[]) {


	int idx=0,x,i;
	disco *z;
	unsigned char tt;
	

	for(idx=0;idx<argc;idx++)
		printf("\nargv[%d] %s",idx,argv[idx]);
	if(argc==1){
		printf("\nno file name...");
		exit(EXIT_FAILURE);
	}
	if(argc==3){
		idxdir=atoi(argv[2]);
		printf("\ndump for %d in dir \n",idxdir);
	}

	FILE* in_file = fopen(argv[1], "rb");
    if (!in_file) {
        perror("fopen");
        exit(EXIT_FAILURE);
    }

	//printf("\nsizoof di disco %u",sizeof(disco));//174848
	//tt = 256*(21*17 +19*7 +18*6 + 17*5);

	z = malloc( 256*(21*17 +19*7 +18*6 + 17*5) ) ;//174848
	fread(z, 256, (21*17 +19*7 +18*6 + 17*5), in_file);


	printf("\n---- track 18 -----");
	printf("\nfirst sector of directory entry track:%u sector:%u\n",z->bam.info[0],z->bam.info[1]);
	printf("\n     format used: %c",z->bam.info[2]);
	//printf("\nunused: %c",(d64+tr18)->info[3]);
	printf("\n           label: ");
	for(i=0;i<16;i++){
		tt=z->bam.label[i];
		if(tt==0xa0) 
			tt=32;
		printf("%c",tt);
	}

	printf("\ndiskette id  esa: %02x%02x",z->bam.did[0],z->bam.did[1]);
	printf("\ndiskette id char: %c%c",z->bam.did[0],z->bam.did[1]);
	printf("\n     dos version: %c%c",z->bam.dosver[0],z->bam.dosver[1]);
	printf("\n");
	printf("\nBlock Availability Map (BAM)");
	printf("\n1 = free");
	printf("\n0 = allocated");
	printf("\n");


	char tmp[9];
	int bf;
	int tot_bf=0;
	for(i=0;i<35;i++){
		printf("\nBAM TRACK %2u",i+1);
		bf=z->bam.block[i][0];
		tot_bf+=bf;
		printf("   block free: %3u",bf);
		printf("   bit map %02x%02x%02x",z->bam.block[i][1],z->bam.block[i][2],z->bam.block[i][3]);
		tobinary(z->bam.block[i][1],&tmp);
		printf(" %s", tmp);
		tobinary(z->bam.block[i][2],&tmp);
		printf(" %s", tmp);
		tobinary(z->bam.block[i][3],&tmp);
		printf(" %s", tmp);		
		/*printf(" %s %s %s",tobinary(z->bam.block[i][1],&tmp),\
			tobinary(z->bam.block[i][2],&tmp),\
			tobinary(z->bam.block[i][3],&tmp));	*/
	}
	printf("\nblock free %u",tot_bf);

	//dir
	printf("\nDirectory");
	printf("\n----trk-sec-name---------------type--------");
//	printf("\n----trk-sec-name---------------type------------------------------");
	printf("trk-sec-----block------------------");
	printf("\n");

	int nextt=18;
	int nexts=1;
	

	//Read track 18 sector 1 and next in chain (no control for track)
	voice ee;
	idx=1;
	do {
		for(x=0;x<8;x++){
			ee=z->dir[nexts-1].e[x];
			print_voice(ee, idx);
			idx++;
		}	
		nextt=z->dir[nexts-1].e[0].next_track;
		nexts=z->dir[nexts-1].e[0].next_sector;		
	} while (nextt!=0 && nexts!=0xff);
	printf("\n");
	
	
	unsigned char b[257];
	nextt=savet;
	nexts=saves;
	
	printf("\n");
	
	if(idxdir>0){
		idx=1;
		do {
			printf("\ntrack %u sector %u\n",nextt,nexts);
			tt=nextt-1;
			if(tt<18) {
				memcpy(b,&z->track1_17[tt*256*21+nexts*256],256);
				nextt=z->track1_17[tt*256*21+nexts*256];
				nexts=z->track1_17[tt*256*21+1+nexts*256];
			}
			if(tt>17&&tt<24){
				tt=tt-18;
				memcpy(b,&z->track19_24[tt*256*19+nexts*256],256);
				nextt=z->track19_24[tt*256*19+nexts*256];
				nexts=z->track19_24[tt*256*19+1+nexts*256];
			}
			if(tt>23&&tt<30){
				tt=tt-24;
				memcpy(b,&z->track25_30[tt*256*18+nexts*256],256);
				nextt=z->track25_30[tt*256*18+nexts*256];
				nexts=z->track25_30[tt*256*18+1+nexts*256];
			}
			if(tt>29){
				tt=tt-30;
				memcpy(b,&z->track31_35[tt*256*17+nexts*256],256);
				nextt=z->track31_35[tt*256*17+nexts*256];
				nexts=z->track31_35[tt*256*17+1+nexts*256];
			}
			//nextt=z->track31_35[tt*256*17];
			//nexts=z->track31_35[tt*256*17+1];
			//memcpy(b,&z->track31_35[tt*256*17],256);
			printf("\nblock %4d\n",idx);
			idx++;
			dump(b);
		
		
		} while (nextt!=0);
	
	}



/************************************

END

************************************/

    exit(0);
}

void tobinary( unsigned char byte, char *r ){//, char r[9] 
	*(r+8)='\0';		
	*(r+0)=byte & 0x80 ? '1' : '0';
	*(r+1)=byte & 0x40 ? '1' : '0';
	*(r+2)=byte & 0x20 ? '1' : '0';
	*(r+3)=byte & 0x10 ? '1' : '0';
	*(r+4)=byte & 0x08 ? '1' : '0';
	*(r+5)=byte & 0x04 ? '1' : '0';
	*(r+6)=byte & 0x02 ? '1' : '0';
	*(r+7)=byte & 0x01 ? '1' : '0';
//	printf("\n%02x valore=%s",byte,r);
	return;
}


// ---------------------------------------------------------------------
// --trk-sec-name--------------type---trk--sec---block------------------
//   000 000 1234567890123456 ---
void print_voice(voice e,int idx){
	int x,i,tt;
	printf("\n%2d  ", idx);

	printf("%03u",e.next_track);
	printf(" ");
	printf("%03u",e.next_sector);
	printf(" ");
	for(i=0;i<16;i++){
		tt=e.file_name[i];
		if(tt==0xa0) 
			tt=32;
		printf("%c",tt);
	}
	printf("   ");
	printf("%03u",e.file_type);
	
	decode_type(e.file_type);
	
	printf("    ");
	printf("%03u",e.first_track);
	printf(" ");
	printf("%03u",e.first_sector);
	printf("   ");	
	tt=e.num_block[0]+256*e.num_block[1];
	printf("%7u",tt);
	if(idx==idxdir){
		savet=e.first_track;
		saves=e.first_sector;
	}

}

void dump(unsigned char *b) {
	unsigned char ascii[17];
	ascii[16]='\0';
	int idx=0,x,i;
	for(i=0;i<256;i++){
		x=b[i];
		printf("%02x ",x);
		if(x>=32&&x<=127) 
			ascii[idx++]=x;
		else 
			ascii[idx++]='.';
		x=i+1;
		if((x-(x/16)*16)==0){
			printf(" %s",ascii);
			idx=0;
			printf("\n");
		}
	}	

}
void decode_type( unsigned char x ) {
// 0x00
// 0x80
// 0xa0
// 0xc0
	char s0[4];
	char s1[20];
	char s2[20];


	
	switch (x&0x0f){
		case 0:
			strcpy(s2,"deleted");
			strcpy(s0,"DEL");
			break;
		case 1:
			strcpy(s2,"sequential");
			strcpy(s0,"SEQ");
			break;
		case 2:
			strcpy(s2,"program");
			strcpy(s0,"PRG");
			break;
		case 3:
			strcpy(s2,"user");
			strcpy(s0,"USR");
			break;
		case 4:
			strcpy(s2,"relative");
			strcpy(s0,"REL");
			break;
		default:
			strcpy(s2,"other");
			strcpy(s0,"");
			break;
	}
	
		
	switch (x & 0xf0){
		case 0:
			strcpy(s1,"Unclosed");			
			break;
		case 0x80:
			strcpy(s1,"");
			break;
		case 0xa0:
			strcpy(s1,"@ replacement");
			break;
		case 0xc0:
			strcpy(s1,"locked");
			break;
		default:
			strcpy(s1,"other");
			break;
												
	}
	
	printf(" %4s", s0, s1, s2);
//	printf(" %4s%11s%11s", s0, s1, s2);
}

