# readfloppyimg
Just for fun and to remember the language c. 
Two simple programs to read the images of the old msdos (fat12) and commodore (d64) floppy disks 
created perhaps using http://cowlark.com/fluxengine/ 
or with some other utilities such as dd.


example

read_fat12.exe dos211.img

LEN 368640

Bios parameters block
 byte_for_sector       512
 sector_for_cluster    2
 sector_reserved       1
 fat_number            2
 max_voice             112
 sector_total          720
 fd                    fd
 sector_for_fat        2
 check                 aa55

--------------------------------------------- start

\
1    MSDOS.SYS    RHS..A.. 10:01:22 21/09/1985   17176 [2] >> 3
2    ERSOIO.BIN   RHS..A.. 10:01:26 21/09/1985    6836 [19] >> 20
3    COMMAND.COM  .....A.. 12:03:16 10/11/1983   15957 [26] >> 27
4    PRINT.COM    .....A.. 00:07:18 01/01/1980    4688 [42] >> 43
5    EDLIN.COM    .....A.. 19:51:10 19/10/1983    8080 [47] >> 48
6    CHKDSK.COM   .....A.. 19:51:12 19/10/1983    6468 [55] >> 56
7    SYS.COM      .....A.. 00:08:54 01/01/1980     950 [62] >> 4095
8    DISKCOPY.COM .....A.. 01:41:14 01/01/1980    2444 [63] >> 64
9    RECOVER.COM  .....A.. 19:51:42 19/10/1983    2295 [65] >> 66
10   MORE.COM     .....A.. 19:51:46 19/10/1983    4364 [68] >> 69
11   DEBUG.COM    .....A.. 19:52:14 19/10/1983   12223 [73] >> 74
12   LIB.EXE      .....A.. 19:52:04 19/10/1983   61696 [85] >> 86
13   FIND.EXE     .....A.. 19:51:20 19/10/1983    6331 [146] >> 147
14   FC.EXE       .....A.. 19:51:24 19/10/1983    2585 [153] >> 154
15   LINK.EXE     .....A.. 19:51:56 19/10/1983   42330 [156] >> 157
16   EXE2BIN.EXE  .....A.. 19:51:18 19/10/1983    1649 [198] >> 199
17   EXEFIX.EXE   .....A.. 19:52:32 19/10/1983   11776 [200] >> 201


.....

read_d64.exe Commodore_1541_1983_Commodore_test_demo-diskette.d64

argv[0] read_d64.exe
argv[1] Commodore_1541_1983_Commodore_test_demo-diskette.d64
---- track 18 -----
first sector of directory entry track:18 sector:1

     format used: A
           label: 1541TEST/DEMO
diskette id  esa: 5a58
diskette id char: ZX
     dos version: 2A

Block Availability Map (BAM)
1 = free
0 = allocated

BAM TRACK  1   block free:  21   bit map ffff1f 11111111 11111111 00011111
BAM TRACK  2   block free:  21   bit map ffff1f 11111111 11111111 00011111
BAM TRACK  3   block free:  21   bit map ffff1f 11111111 11111111 00011111
BAM TRACK  4   block free:  21   bit map ffff1f 11111111 11111111 00011111
BAM TRACK  5   block free:  21   bit map ffff1f 11111111 11111111 00011111
BAM TRACK  6   block free:  21   bit map ffff1f 11111111 11111111 00011111
BAM TRACK  7   block free:  21   bit map ffff1f 11111111 11111111 00011111
BAM TRACK  8   block free:  21   bit map ffff1f 11111111 11111111 00011111
BAM TRACK  9   block free:  21   bit map ffff1f 11111111 11111111 00011111
BAM TRACK 10   block free:  21   bit map ffff1f 11111111 11111111 00011111
BAM TRACK 11   block free:  21   bit map ffff1f 11111111 11111111 00011111
BAM TRACK 12   block free:  21   bit map ffff1f 11111111 11111111 00011111
BAM TRACK 13   block free:  21   bit map ffff1f 11111111 11111111 00011111
BAM TRACK 14   block free:  17   bit map d75f1f 11010111 01011111 00011111
BAM TRACK 15   block free:   0   bit map 000000 00000000 00000000 00000000
BAM TRACK 16   block free:   0   bit map 000000 00000000 00000000 00000000
BAM TRACK 17   block free:   0   bit map 000000 00000000 00000000 00000000
BAM TRACK 18   block free:  16   bit map ecff07 11101100 11111111 00000111
BAM TRACK 19   block free:   0   bit map 000000 00000000 00000000 00000000
BAM TRACK 20   block free:   0   bit map 000000 00000000 00000000 00000000
BAM TRACK 21   block free:  18   bit map bfff07 10111111 11111111 00000111
BAM TRACK 22   block free:  19   bit map ffff07 11111111 11111111 00000111
BAM TRACK 23   block free:  19   bit map ffff07 11111111 11111111 00000111
BAM TRACK 24   block free:  19   bit map ffff07 11111111 11111111 00000111
BAM TRACK 25   block free:  18   bit map ffff03 11111111 11111111 00000011
BAM TRACK 26   block free:  18   bit map ffff03 11111111 11111111 00000011
BAM TRACK 27   block free:  18   bit map ffff03 11111111 11111111 00000011
BAM TRACK 28   block free:  18   bit map ffff03 11111111 11111111 00000011
BAM TRACK 29   block free:  18   bit map ffff03 11111111 11111111 00000011
BAM TRACK 30   block free:  18   bit map ffff03 11111111 11111111 00000011
BAM TRACK 31   block free:  17   bit map ffff01 11111111 11111111 00000001
BAM TRACK 32   block free:  17   bit map ffff01 11111111 11111111 00000001
BAM TRACK 33   block free:  17   bit map ffff01 11111111 11111111 00000001
BAM TRACK 34   block free:  17   bit map ffff01 11111111 11111111 00000001
BAM TRACK 35   block free:  17   bit map ffff01 11111111 11111111 00000001
block free 574
Directory
----trk-sec-name---------------type--------trk-sec-----block------------------

 1  018 004 HOW TO USE         130  PRG    017 000        13
 2  000 000 HOW PART TWO       130  PRG    017 003         5
 3  000 000 VIC-20 WEDGE       130  PRG    017 009         4
 4  000 000 C-64 WEDGE         130  PRG    019 000         1
 5  000 000 DOS 5.1            130  PRG    019 001         4
 6  000 000 COPY/ALL           130  PRG    019 003        11
 7  000 000 PRINTER TEST       130  PRG    019 009         9
 8  000 000 DISK ADDR CHANGE   130  PRG    016 000         4
 9  000 255 DIR                130  PRG    016 001         4
10  000 000 VIEW BAM           130  PRG    016 003         6
11  000 000 CHECK DISK         130  PRG    016 007         4
12  000 000 DISPLAY T&S        130  PRG    016 015        14
13  000 000 PERFORMANCE TEST   130  PRG    020 002         9
14  000 000 SEQUENTIAL FILE    130  PRG    020 007         5
15  000 000 RANDOM FILE        130  PRG    015 001        13
16  000 000                    000  DEL    000 000         0


