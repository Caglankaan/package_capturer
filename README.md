# Package Capturer

## Dependencies
This program uses pcap.h so you first have to install it.

```bash
sudo apt-get install libpcap-dev
```

## Installation

```bash
mkdir build
cd build
cmake ..
make
```

## Usage

```bash
sudo ./executable -r <valid_regex> -t <thread_count>
```
Program takes 2 inputs. Valid regex and thread count. There are 10 thread limit. After we start program it starts to capture network packages with properties: 
1- Destination Address
2- Source Address
3- Source IP
4- Destination IP
5- Protocol
6- Source Port
7- Destination Port
8- Data Payload

If we give any regex expression it starts to search for that regex expression in our output and if there are any match it prints data to the screen. After we start looping and capturing packages this program sends them to the thread pool.

Example usage:
```bash
sudo ./executable -r 1.1.172.[0-9]{1,3} -t 3
```
So this creates a thread pool with 3 thread (if we dont give -t tag it creates 5 thread by default) and filters our output with ip address 1.1.172.x.
Example output: 
```bash
--------------------------------------------------------
Destionation Address    : B8-9A-2A-59-67-E4
Source Address          : E4-A7-49-EF-24-12
Source IP               : 74.125.11.71
Destination IP          : 172.17.6.19
Protocol                : 8
Source Port             : 443
Destination Port        : 56856
Data Payload            :
   2471161884710823539176110149127132869117454         .t./l..n..V[.6
   1851161301931711067474130234100205541460         .t...jJJ..d.....
   8624521178240451289110923124615813020320748         V..N.-�[m......0
   64481556352502272550632394290181124191         @0.8#....?.*Z.|.
   12921477801681347619531077611921718176         ..MP..L..kL....L
   207192541171458119710623811811221621820426167         ...u.Q.j.vp.....
   13104991342207516845102249845715922424318         .hc..K.-f.T9....
   1516518913278651061133130220105202172128255         ....NAj....i..�.
   2091531571572321521617781242312343920969153         ........Q....E.
   193606922421597201188212051332317013186133         .<E..a......F...
   216982371342478458111245912021213018968         ..b...T:.|;x...D
   ...
   ...
--------------------------------------------------------
```

Thread count:

```bash
kaancaglan@pop-os:~/development$ cat /proc/53915/status | grep Threads
Threads:	1
kaancaglan@pop-os:~/development$ cat /proc/53915/status | grep Threads
Threads:	2
kaancaglan@pop-os:~/development$ cat /proc/53915/status | grep Threads
Threads:	4
kaancaglan@pop-os:~/development$ cat /proc/53915/status | grep Threads
Threads:	1
```