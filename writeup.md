# HACKvent 2017
write-up by: muetho

## day 1
| CHALLENGE DESCRIPTION                                                                                                     |
|---------------------------------------------------------------------------------------------------------------------------|
| 5th anniversary - time to have a look back                                                                                |

| DOWNLOADS (FILES / LINKS)                                                                                                 |
|---------------------------------------------------------------------------------------------------------------------------|
| [HV17-hv16-hv-15-hv14.svg](day01/HV17-hv16-hv15-hv14.svg)                                                                 |

| HINTS                                                                                                                     |
|---------------------------------------------------------------------------------------------------------------------------|
| -                                                                                                                         |

### solution

* went to https://github.com/shiltemann/CTF-writeups-public and get write up from past years HACKvent to get the specific nuggets needed
* 2014-12-01 Nugget: 	HV24-BAAJ-6ZtK-__IJHy__-bABB-YoMw
* 2015-12-01 Nugget: 	HV15-Tz9K-4JIJ-EowK-__oXP1__-NUYL
* 2016-12-01 Nugget: 	HV16-t8Kd-38aY-QxL5-bn4K-__c6Lw__
* Nugget this year: 	__HV17-5YRS-4evr-IJHy-oXP1-c6Lw__

## day 2
* base64 - -decode 32 times on wishlist.txt
* Nugget: __HV17-Th3F-1fth-Pow3-r0f2-is32__

## day 3
* DEBUG message in logcat.txt line 2681 says: Failed to send raw PDU message
* Searched for raw PDU string: logcat.txt line 2749: 07914400000000F001000B913173317331F300003AC7F79B0C52BEC52190F37D07D1C3EB32888E2E838CECF05907425A63B7161D1D9BB7D2F337BB459E8FD12D188CDD6E85CFE931
* decoded with online SMS PDU Decode/Encode: https://www.diafaan.com/sms-tutorials/gsm-modem-tutorial/online-sms-pdu-decoder/
* Message payload: “Good Job! Now take the Flag: __HV17-th1s-isol-dsch-00lm-agic”__
## day 4
* PDF ‘HoHoHo\_medium.pdf’ contains attachment ‘DroidSans-HACKvent.sfd’
* Open .sfd file in FontForge (downloaded via apt-get install fontforge)
* Choose File > Generate Fonts and generate (ignore all warnings)
* Open generated file ‘DroidSans-Regular.pfb’
* Nugget can be readout starting at character 45: __HV17-RP7W-DU6t-Z3qA-jwBz-jItj__

## day 5
* Given flags are CRC32 checksums of the actual ASCII flags
* Brute force CRC32 calculation with crc32\_brute\_force.c (attachment)
* Nugget: __HV17-7pKs-whyz-o6wF-h4rp-Qlt6__

## day 6
* Follow link in challenge to http://challenges.hackvent.hacking-lab.com:4200/
* Scan QR Code return country names 
* Python script (attachment) to repeat request multiple times and decode QR code (qrtools)
* After some iterations, flag is returned instead of another (random) country name
* Nugget: __HV17-eCFw-J4xX-buy3-8pzG-kd3M__

## day 7
* Download SANTA.FILE
* open file manager and extract the SANTA.FILE archive → SANTA.IMA file extracted
* hexdump -C of SANTA.IMA contains flag in lines 42215 – 42217 (in ASCII columns)
* Nugget: __HV17-Ucyz-0yEU-d90O-vSqS-Sd64__

## day 8
* Download file True.1337 → starts with exec() statements and contains multiple chr() statements. Find programming language containing these commands. PHP and Python are possible candidates, but the upper case keyword ‘True’ implies it’s Python (also the challenge was created by pyth0n33)
* run script with python2.7 fails but prints the command within the first exec statement:
```
A=chr;__1337=exec;SANTA=input;FUN=print
def _1337(B):return A(B//1337)
```
* This is the key for deobfuscation of the second part containing the 1337s
* Run script with python3 executes without error and print “?” to stdout waiting for input
* Deobfuscation of second statement by hand (vim substitution) for \_\_1337 to print (instead exec to see the command to be executed), \_1337 to chr and 1337 to 1 (or True) and execute script shows:
```
C=SANTA("?")
if C=="1787569":FUN(''.join(chr(ord(a) ^ ord(b)) for a,b in zip("{<some non-ASCII characters>","31415926535897932384626433832")))
```
* The flag to provide as input after execute with python3 therefore is  1787569 what let the script return the flag
* Nugget: __HV17-th1s-ju5t-l1k3-j5sf-uck!__

## day 9
* unzip the JSONionzip
* print the JSON file with python -m json.tool jsonion.json
* JSON object has fields:
  * op = map
  * mapFrom = \<string\>
  * mapTo = \<string\>
  * content = \<string\>
* execute python script for mapping the characters of string “content” from “mapFrom” to “mapTo” reveals a new JSON object with op = gzip
* execute python script for unzip the string “content” reveals another JSON object with op=b64
* The task is to apply the operation “op” onto the content, different operations:
  * map: map characters in “content” from “mapFrom” to characters in “mapTo”
  * gzip: unzip the (base64 decoded) “content”
  * b64: base64  decoding of “content”
  * xor: XOR each character of the (base64 decoded) “content” with the (base64 decoded) “mask”
  * rev: reverting order of “content” (last character is 1st character …)
  * nul: string “content” can directly be used as next JSON object
* for each operation there is one python script (attachment), iterating through the different layers of the onion was done with piping of stdout by hand, this was time-consuming (specially the map operation on long contents) but helped to detect directly that in layer 74 there are two sub objects (op=nul and op=xor), these were splitted to further process their content.
* The XOR-branch (first sub object of layer 74) lead to the last layer (after 8 more iterations) whit op=flag and content="THIS-ISNO-THEF-LAGR-EALL-Y…", so the other branch (2nd sub object of layer 74 with op=nul) had to be followed.
* Apply the operations repeatedly on the 2nd branch revealed the flag after 20 more pipe stages
* Nugget: __HV17-Ip11-9CaB-JvCf-d5Nq-ffyi__

## day 10
* Use netcat (nc) to connect to the tic-tac-toe game server as described in challenge description
* Playing some games reveals: losing the game or entering an invalid key will cancel the connection, winning the game 1st time shows 1/100, after disconnect the counter is reset to 0. Only winning or stand-off will keep the connection alive.
* Tactics to win tic-tac-toe says start with a corner, in the next move set the opposite corner and in the third move set one of the remaining corner.
* There is a combination which always wins the game: 7-3-9-6
* Automating strings passing via stdin: 
```
 perl -e 'print "\n7\n3\n9\n6\n" x 100' - | nc challenges.hackvent.hacking-lab.com 1037
```
* After 100 wins, the password is print to sdtout
* Nugget: __HV17-y0ue-kn0w-7h4t-g4me-sure__

## day 11
* using sagemath (online math cloud at www.cocalc.com), a python based math environment with builtin functions perfectly suitable for crypto (power\_mod,…)
```
c = (a\*b) mod p -> (c mod p) = ((a mod p) \* (b mod p)) mod p -> (c mod p) / (b mod p) = a mod p 

```
* since c < p, a < p: 

```
a = c / (b mod p) = 49924947538335498107122362916688669178570808432525920972483661185623270410 = 485631372d7a51427a2d417744672d3146454c2d725545392d474b67710016
```
* ASCII decoding of a reveals flag
* Nugget: __HV17-zQBz-AwDg-1FEL-rUE9-Gkgq__

## day 12

## day 13
* Download the file muffin\_asm.py
* Running it: Script waits for flag provided via stdin, providing the wrong flag returns “[-] nope” and terminates script
* Analyzing the script: Different logic operations are defined (ADD,SUB,XOR,JE,CMP,…) collected in an instruction vector “ins”, the method “run” performs kind of an Arithmetic Logic Unit (ALU) on a provided bytecode. The bytecode is defined at the end of the file (codez)
* Based on these findings it is clear that the characters of the flag needs to be stored somewhere within the bytecode and there must be a comparison of the provided flag via stdin and the hardcoded flag. 
* The funtion \_cmp compares the provided char with the hardcoded, printing both values (r[r1] and r[r2]) reveals that at the offset r1 the provided value is stored where the character of the flag is stored at offset r2.
* Conditional jumps (e.g. if equal, jump else not) are realized with the ALU functions \_je (jump if equal) and \_jne (jump if not equal), exchanging \_je and \_jne in the instruction array “ins” tricks the run method to behave as if the correct flag were provided even if an arbitrary character is given as input.
* This exchange combined with the print of the character in r[r2] changes the behavior of the script to printing out the flag when (at least) 24 arbitrary characters are provided as input. To make sure the \_cmp instruction returns false (to perform the jump), the input characters needs to be chosen so that they are not included in the flag, I tried with ‘\*’ and it succeeded.
* See edited script in attachment
* Nugget: __HV17-mUff!n-4sm-!s-cr4zY__

## day 14
* Download the happy\_cryptmas.zip and unpack → file hackvent
* hexdump -C hackvent → this seems to be an exe binary
* strings hackvent → stringing the hackvent file reveals (amongst other more or less meaningless strings) a 512 bit hex number and 65537, these could be the public modulus n and the public exponent e of an RSA encryption
* the provided flag in the challenge description (also 512 bit) then should be the encrypted ciphertext c
* RSA encryption: 
c = m<sup>e</sup> mod n
* RSA decryption: 
m = c<sup>d</sup> mod n
* relation between public exponent e and private exponent d: where p and q are the primefactors of n: and (p-1)\*(q-1) denotes the euler phi function of n (number of elements that do not divide n): when both p and q are prime
* to calculate the cleartext message m (which is presumably the flag) we need the private exponent d, which is the modular inverse of e and the prime factorization of n or the euler phi function of n (programming effort is equally for both)→ again using sagemath via online cloud cocalc.com
* given:
```     
c = 0x7A9FDCA5BB061D0D638BE1442586F3488B536399BA05A14FCAE3F0A2E5F268F2F3142D1956769497AE677A12E4D44EC727E255B391005B9ADCF53B4A74FFC34C
n = 0xF66EB887F2B8A620FD03C7D0633791CB4804739CE7FE001C81E6E02783737CA21DB2A0D8AF2D10B200006D10737A0872C667AD142F90407132EFABF8E5D6BD51
e = 65537
```
* calculated:
```
euler_phi(n) = 0xf66eb887f2b8a62002517574bb3399028396afc1fdc465d0566317f20bf33da2e917e1e6be3ed3c3cfb32ff4830181c84a4112e882a8c27339d7db84c02d9bb4
d = power_mod(e,-1,euler_phi(n)) = 0xf66eb887f2b8a62002517574bb3399028396afc1fdc465d0566317f20bf33da2e917e1e6be3ed3c3cfb32ff4830181c84a4112e882a8c27339d7db84c02d9bb4
m = power_mod(c,d,n) = 0x485631372d35424d752d6d6744302d473753752d455973702d4d673062
```
* ASCII decoding of m reveals flag
* Nugget: __HV17-5BMu-mgD0-G7Su-Eysp-Mg0b__

## day 15

## hidden 1 
* Try to access https://hackvent.hacking-lab.com/challenge.php?day=1984
* Message says: “The resource you are trying to access, is hidden in the header.”
* Enable ZAP Proxy, resend the GET request to day 1984 and readout flag in response header
* Nugget: __HV17-4llw-aysL-00ki-nTh3-H34d__
