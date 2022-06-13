#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int count_voice=0;
int global=0;


typedef struct {
	char a[128];
} path;
path root[20];

void scan_dir(unsigned char *all_file, int nsec,    int posiz,    int ns,unsigned char *fat12) ;
int find_cluster(int nc,unsigned char *fat12) ;
#define GETDW(_v_) ( (_v_[1] << 8) | _v_[0]  |   (_v_[2] *65536 )  | (_v_[3] *16777216)        )
#define GETW(_v_) ( (_v_[1] << 8) | _v_[0])
#define GETB(_v_) ( (_v_[0]) )

#define GEThh(_v_) ( (_v_ & 0xf800) >> 11)
#define GETmm(_v_) ( (_v_ & 0b0000011111100000) >> 5)
#define GETss(_v_) ( (_v_ & 0x001f) )

#define GETy(_v_) ( (_v_ & 0b1111111000000000) >> 9)
#define GETm(_v_) ( (_v_ & 0b0000000111100000) >> 5)
#define GETd(_v_) ( (_v_ & 0b0000000000011111) )

typedef struct {
	unsigned char jmp[3]; // jmp
	unsigned char name[8]; // name
	unsigned char byte_for_sector[2]; // bytes for sector
	unsigned char sector_for_cluster[1]; // sector for cluster
	unsigned char sector_reserved[2]; // sector reserved
	unsigned char fat_number[1]; // fat number 
	unsigned char max_voice[2]; // max voice for root
	unsigned char sector_total[2]; // sector total
	unsigned char fd[1]; // descriptor
	unsigned char sector_for_fat[2]; // sector for fat
	unsigned char sector_for_trace[2]; // sector for trace
	unsigned char head_number[2]; // head number
	unsigned char sector_hidden[4]; // sector hidden
	unsigned char sector_tot[4]; // total sector
	unsigned char filler[474]; //
	unsigned char check[2]; // check 55aa 
} b;

typedef struct {
	unsigned char fat; // jmp[3]
} f12;

typedef struct {
	unsigned char name[8]; // name
	unsigned char ext[3]; // extention
	unsigned char attrib[1]; // 
	/*
	11      Attribute - a bitvector. 
	Bit 0: read only. 
	Bit 1: hidden.
    Bit 2: system file. 
	Bit 3: volume label. 
	Bit 4: subdirectory.
    Bit 5: archive. 
	Bits 6-7: unused.
        "rvAdLSHR";
        */
	unsigned char reserved[1]; // reserved
	unsigned char hour[1]; // 
	unsigned char create_time[2]; // 
	unsigned char create_date[2]; // 
	unsigned char access_date[2]; // access date
	unsigned char first_clusterx[2]; // 
	unsigned char last_time[2]; // 
	unsigned char last_date[2]; // 
	unsigned char first_cluster[2]; // sector hidden
	unsigned char size[4]; // total sector
	//unsigned char size2[2]; // total sector
} d;

b boot_record[1];
d dir_sector[16];
d dir;

// global
int foundit=0,sizefile;
unsigned int sector_for_cluster;
unsigned int byte_for_sector;
unsigned int sector_for_fat;
unsigned int fat_number;
unsigned int sector_total;
unsigned int sector_reserved;
unsigned int max_voice;
unsigned char fd;

int n_file=1;
int idxfile=0;
char idxdump[10];
char idxname[128];

char search_file[]="12345678.123";
int typefat=0;	

int main(int argc, char *argv[]) {
	int idx=0;
	int x;
	int i;

 	//if(argc==3)
 		//strcpy(search_file, argv[2]);
	printf("\nRead FAT12 disk image ver. 0.1 / june 2022\n\n");

	if(argc>2){
		idxfile=atoi(argv[2]);
		//strcpy(idxdump,argv[3]);
		printf("\ndump for %d in dir \n",idxfile);
	}
 	
	//printf("\nargc %d %s",argc,search_file);
	//for(idx=0;idx<argc;idx++)
	//	printf("\nargv[%d] %s",idx,argv[idx]);
	if(argc==1){
		printf("\nno file name");
		exit(EXIT_FAILURE);
	}

	FILE* f = fopen(argv[1], "rb");
    if (!f) {
        perror("fopen");
        exit(EXIT_FAILURE);
    }
    

	fseek(f, 0, SEEK_END); 
	unsigned long len = (unsigned long)ftell(f); 
	printf("\nLEN %ld\n", len); 
	fseek(f, 0, SEEK_SET); 

	unsigned char *all_file;
	all_file = malloc(len);
	fread(all_file, len, 1, f);
	memcpy(boot_record,all_file,512);
	fclose(f); 

	sector_total      = GETW( boot_record[0].sector_total);
	sector_reserved   = GETW( boot_record[0].sector_reserved);
	sector_for_cluster= GETB( boot_record[0].sector_for_cluster);
	byte_for_sector   = GETW( boot_record[0].byte_for_sector);
	sector_for_fat    = GETW( boot_record[0].sector_for_fat );
	fat_number        = GETB( boot_record[0].fat_number );
	max_voice         = GETW( boot_record[0].max_voice );
	fd                = GETB( boot_record[0].fd );
	
	printf("\nBios parameters block");
	printf("\n byte_for_sector       %u",byte_for_sector );
	printf("\n sector_for_cluster    %u",sector_for_cluster );
	printf("\n sector_reserved       %u",sector_reserved );
	printf("\n fat_number            %u",fat_number );
	printf("\n max_voice             %u",max_voice );
	printf("\n sector_total          %u",sector_total );
	printf("\n fd                    %x",fd );
	printf(" - ");
	decode_fd( fd );
	printf("\n sector_for_fat        %x",sector_for_fat );	
	printf("\n check                 %x",GETW( boot_record[0].check ));	

	if(GETW( boot_record[0].check )!=0xaa55){
		printf("\nincorrect boot record");
		
	sector_total      = 720;
	sector_reserved   = 1;
	sector_for_cluster= 1;
	byte_for_sector   = 512;
	sector_for_fat    = 2;
	fat_number        = 2;
	max_voice         = 128;//112;
	fd                = 0xfd;
		
		
//        exit(EXIT_FAILURE);
	}

	
	unsigned int nsec;
	
	nsec = (max_voice*32)/byte_for_sector;//sector for root entry
	
	int tmp = (sector_total - sector_reserved - nsec - (sector_for_fat*2) ) /sector_for_cluster;
	if(tmp<4085) 
		typefat=12;
	if(tmp>=4085&&tmp<65525) 
		typefat=16;
	if(tmp>=65525) 
		typefat=32;

		
	unsigned char *fat12 = malloc(sector_for_fat*byte_for_sector);	
	unsigned char *recorda = malloc(byte_for_sector*sector_for_cluster);
	
	memcpy(fat12,all_file+512,sector_for_fat*byte_for_sector);
	
	int a;
	unsigned char last;
	unsigned char enddir=0;	
	int first;
	int n;
	char namecmp[]="12345678.123";
	

	first=0;

	printf("\n");
	int posiz;

	if(typefat!=12){
		printf("\nonly fat12 ...........");
		exit(EXIT_FAILURE);
	}




	// **read file sector_for_cluster
	printf("\n--------------------------------------------- start\n");
	posiz =  1 + sector_for_fat*fat_number;// boot + nfat*settori * fat

	strcpy(root[global].a,"\\");
	printf("\n%s",root[global].a);

	scan_dir(all_file, nsec, posiz, nsec,fat12);
	
	printf("\n--------------------------------------------- end\n");
	

	int chain;
	idx=100;//maxxxxx
	first=foundit;
	FILE *fout;
	if(foundit>0) {
	
		fout = fopen(idxname, "wb");
		printf("\nDUMP FILE %s",idxname);
		printf("\nnow find the FAT chain, ");
		chain=find_cluster(first,fat12);
		printf("first value for %u = %u \n\n",first,chain);		
	}
	while(first!=0 && idx>0 && sizefile>0){
		idx--;
		chain=find_cluster(first,fat12);



		first--;
		first--;
		first=first*sector_for_cluster;
		first=	first  + 1 + sector_for_fat*fat_number + nsec; 
		first=	first * byte_for_sector;

		//printf("\n%u %u\n",first, fat_number);
		//printf("\nsize:%u \n",sizefile);



		//while (sizefile>0) {
			//memcpy(recorda,all_file+first,byte_for_sector*sector_for_cluster   );
			int is=0;
			while(sizefile>0 && is<(byte_for_sector*sector_for_cluster)){
				//printf("%c",*(all_file+first+is));	
				sizefile--;
				unsigned char bu=*(all_file+first+is);
				fwrite(&bu,1,1,fout);
				is++;
			}
	//	}
		if(chain==4095) break;
		first=chain;
		
	}
	if(foundit>0)
		fclose(fout);
	
	free(all_file);
	free(recorda);
	free(fat12);

	//system("pause");
	return 0;
}



// **************************************
// *
// *
// **************************************
void scan_dir(unsigned char *all_file, int nsec,    int posiz,    int ns,unsigned char *fat12) {

	int idx;
	int i,a,directory;
	unsigned char last;
	unsigned char enddir=0;	
	int first;
	int n;unsigned char xx;
	char namecmp[]="12345678.123";
	
	d *dir_sector=malloc(sizeof(d)*16);
	
	first=0;
	last=0;
	unsigned char filename[9],fileext[4];
	filename[8]='\0';
	fileext[3]='\0';
	
	for(n=0;n<nsec;n++) {

		a=0;
		//fread(dir_sector, byte_for_sector, 1, in_file);
		memcpy(dir_sector,all_file+byte_for_sector*(posiz+n), byte_for_sector);
		
		while (a<16 && enddir==0 && last==0) {
			dir = dir_sector[a];
			directory=0;
	
			if(dir.name[0]==0) {				
				last=1;
				break;
			}
			if(dir.name[0]==0xe5) {				
				//file del
				xx='?';
			}		
			
			printf("\n");
			count_voice++;
			// merge name
			memcpy(filename,dir_sector[a].name,8);
			memcpy(fileext,dir_sector[a].ext,3);
	
			idx=0;		
			for(i=0;i<8;i++){
				if(filename[i]!=32)
					namecmp[idx++]=filename[i];	
			}
			for(i=0;i<3;i++) {
				if(fileext[i]!=32) {
					if(i==0) namecmp[idx++]='.';	
					namecmp[idx++]=fileext[i];	
				}
			}
			namecmp[idx++]='\0';	
			/*
			if ((strcmp(namecmp,search_file))==0 ) {
				foundit=GETW(dir.first_cluster);
				if(foundit!=0) {
					sizefile=GETDW(dir.size);
					printf("found ");
				}
			}
			*/
			if(idxfile==n_file){
				foundit=GETW(dir.first_cluster);
				if(foundit!=0) {
					sizefile=GETDW(dir.size);
					strcpy(idxname,namecmp);		
					printf("found ");
				}						
			}
			
			printf("%-4d ", n_file);
			n_file++;
			printf("%-12s",namecmp);
			// attrib
			unsigned char x;
			unsigned int y,w,q;
			char *s = "RHSLdAvr";
			//char s[8]={'R','H','S','L','S','A','D','r'};
			printf(" ");
			x=dir.attrib[0];
			//printf(" attrib: %x ",x);
			unsigned char z;
			if(x&0x10 && dir.name[0]!='.') 
				directory=1;
			for(i=0;i<8;i++) {
				z=x&0x01;
				if(z)
					printf("%c",s[i]);
				else
					printf(".");
				x=x>>1;
			}
			y=GETW(dir.last_time);
			printf(" %02u:%02u:%02u",GEThh( y ),GETmm( y ),GETss( y )*2);
			y=GETW(dir.last_date);
			printf(" %02u/%02u/%04u",GETd( y ),GETm( y ),1980 + GETy( y ));
			y=GETDW(dir.size);
			printf(" %7u", y);
	
			printf(" [%u]",GETW(dir.first_cluster));
			
			int chain=find_cluster(GETW(dir.first_cluster),fat12);
			printf(" >> %u",chain);
			a++;
			
			if(directory==1) {
				int first;
				first=GETW(dir.first_cluster);
				int prox;
				global++;
				strcpy(root[global].a,namecmp);
				printf("\n");
				for(i=0;i<=global;i++){
					printf("%s\\",root[i].a);
				}	
				do{
					prox=find_cluster(first,fat12);
					first--;
					first--;
					first=first*sector_for_cluster;
					first=	first  + 1 + sector_for_fat*fat_number + ns; 
					directory=0;
					scan_dir(all_file, sector_for_cluster,  first, ns,fat12) ;
				
					first=prox;
				}while(prox!=4095);
				global--;
			
			}
		}
		if(last>0) break;
	}
	free(dir_sector);
}

// **************************************
// *
// *
// **************************************
int find_cluster(int nc,unsigned char *fat12) {

	unsigned short aa,bb,c1,z;

	z=nc&0x01;//cc&0x05;
	nc=nc*1.5;
	if(z==0){
		aa=fat12[nc];bb=fat12[nc+1];
		c1=aa + (bb & 0x0f)*256;
		//	printf(" %u nc=%u z=%u aa= %x bb= %x c1= %u",c1,nc,z,aa,bb,c1);
	} else {
		aa=fat12[nc];bb=fat12[nc+1];
		c1=((aa&0xf0)>>4 )+ (bb *16);
	//	printf(" %u nc=%u z=%u aa= %x bb= %x c1= %u",c1,nc,z,aa,bb,c1);
	}

return c1;				


}


void decode_fd( unsigned char fd){
	switch(fd) {
		case 0xf0:
			printf("2.88 MB 3.5-inch, 2-sided, 36-sector / 1.44 MB 3.5-inch, 2-sided, 18-sector");
			break;
		case 0xf9:
			printf("720K 3.5-inch, 2-sided, 9-sector / 1.2 MB 5.25-inch, 2-sided, 15-sector");
			break;
		case 0xfd:
			printf("360K 5.25-inch, 2-sided, 9-sector / 500K 8-inch, 2-sided, single-density");
			break;
		case 0xff:
			printf("320K 5.25-inch, 2-sided, 8-sector");
			break;
		case 0xfc:
			printf("180K 5.25-inch, 1-sided, 9-sector");
			break;
		case 0xfe:
			printf("160K 5.25-inch, 1-sided, 8-sector / 250K 8-inch, 1-sided, single-density / 1.2 MB 8-inch, 2-sided, double-density");
			break;
		case 0xf8:
			printf("Fixed disk");
			break;




		default:
			printf("unknown");
	}
/*	
	F0 2.88 MB 3.5-inch, 2-sided, 36-sector
F0 1.44 MB 3.5-inch, 2-sided, 18-sector
F9 720K 3.5-inch, 2-sided, 9-sector
F9 1.2 MB 5.25-inch, 2-sided, 15-sector
FD 360K 5.25-inch, 2-sided, 9-sector
FF 320K 5.25-inch, 2-sided, 8-sector
FC 180K 5.25-inch, 1-sided, 9-sector
FE 160K 5.25-inch, 1-sided, 8-sector
FE 250K 8-inch, 1-sided, single-density
FD 500K 8-inch, 2-sided, single-density
FE 1.2 MB 8-inch, 2-sided, double-density
Fixed disk
*/
	
	
	
}
