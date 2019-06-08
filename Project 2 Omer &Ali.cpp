#include <iostream>
#include  <iomanip>
#include <cmath>
#include <time.h >
#include<stdlib.h>

using namespace std;



#define		DRAM_SIZE		(64*1024*1024)
#define		CACHE_SIZE		(64*1024)

enum cacheResType { MISS = 0, HIT = 1 };

const char* msg[2] = { "Miss","Hit" };

unsigned int m_w = 0xABABAB55;    /* must not be zero, nor 0x464fffff */
unsigned int m_z = 0x05080902;    /* must not be zero, nor 0x9068ffff */

unsigned int rand_()
{
	m_z = 36969 * (m_z & 65535) + (m_z >> 16);
	m_w = 18000 * (m_w & 65535) + (m_w >> 16);
	return (m_z << 16) + m_w;  /* 32-bit result */
}


unsigned int memGen1()
{
	static unsigned int addr = 0;
	return (addr += (16 * 1024)) % (256 * 1024);
}

unsigned int memGen2()
{
	static unsigned int addr = 0;
	return (addr += 4) % (DRAM_SIZE);
}

unsigned int memGen3()
{
	return rand_() % (256 * 1024);
}

//Exp 2

class twoLevelCache {

private:
	const int numLines_L1 = 32 * 1024 / 64;
	const int numLines_L2 = 256 * 1024 / 64;

	int L1[512];
	int L2[4096];

	int L1_Speed = 4;
	int L2_Speed = 11;
	int Mem_speed = 100;

public:
	twoLevelCache() {
		for (int i = 0; i < numLines_L1; i++) {
			L1[i] = -1;
		}

		for (int i = 0; i < numLines_L2; i++) {
			L2[i] = -1;
		}
	}


	void Analysis() {

		int hit_L1 = 0, hit_L2 = 0, miss_L1 = 0, miss_L2 = 0, hit_mem = 0;
		double AMAT_L2, AMAT;

		unsigned int addr;

		for (int x = 0; x < 3; x++) {

			hit_L1 = 0; hit_L2 = 0; miss_L1 = 0; miss_L2 = 0; hit_mem = 0;

			for (int i = 0; i < numLines_L1; i++) {
				L1[i] = -1;
			}

			for (int i = 0; i < numLines_L2; i++) {
				L2[i] = -1;
			}


			for (int i = 0; i < 1000000; i++)
			{

				switch (x) {
				case 0:
					addr = memGen1();
					break;
				case 1:
					addr = memGen2();
					break;
				default:
					addr = memGen3();
				}

				int effAddr = addr / 64;

				if (L1[effAddr % numLines_L1] == effAddr) {

					L1[effAddr % numLines_L1] = effAddr;
					L2[effAddr % numLines_L2] = effAddr;

					hit_L1++;
				}

				else if (L2[effAddr % numLines_L2] == effAddr) {

					L1[effAddr % numLines_L1] = effAddr;
					L2[effAddr % numLines_L2] = effAddr;

					hit_L2++;
					miss_L1++;

				}
				else {

					L1[effAddr % numLines_L1] = effAddr;
					L2[effAddr % numLines_L2] = effAddr;

					hit_mem++;
					miss_L1++;
					miss_L2++;

				}
			}

			cout << hit_L1 << ' ' << miss_L1 << endl;
			cout << hit_L2 << ' ' << miss_L2 << endl;


			AMAT_L2 = L2_Speed + (double(miss_L2) / double(miss_L1)) * Mem_speed;
			AMAT = L1_Speed + (double(miss_L1) / 1000000) * AMAT_L2;

			cout << x + 1 << ' ' << AMAT << endl;

			cout << endl;


		}



	}

};

class threeLevelCache {

private:
	const int numLines_L1 = 32 * 1024 / 64;
	const int numLines_L2 = 256 * 1024 / 64;
	const int numLines_L3 = 8192 * 1024 / 64;


	int* L1 = new int[numLines_L1];
	int* L2 = new int[numLines_L2];
	int* L3 = new int[numLines_L3];


	int L1_Speed = 4;
	int L2_Speed = 11;
	int L3_Speed = 30;
	int Mem_speed = 100;

public:
	threeLevelCache() {
		for (int i = 0; i < numLines_L1; i++) {
			L1[i] = -1;
		}
		for (int i = 0; i < numLines_L2; i++) {
			L2[i] = -1;
		}
		for (int i = 0; i < numLines_L3; i++) {
			L3[i] = -1;
		}
	}


	void Analysis() {

		int hit_L1 = 0, hit_L2 = 0, miss_L1 = 0, miss_L2 = 0, hit_L3 = 0, miss_L3 = 0, hit_mem = 0;

		unsigned int addr;
		for (int i = 0; i < 1000000; i++)
		{

			addr = memGen1();

			int effAddr = addr / 64;

			if (L1[effAddr % numLines_L1] == effAddr) {

				L1[effAddr % numLines_L1] = effAddr;
				L2[effAddr % numLines_L2] = effAddr;
				L3[effAddr % numLines_L3] = effAddr;

				hit_L1++;
			}

			else if (L2[effAddr % numLines_L2] == effAddr) {

				L1[effAddr % numLines_L1] = effAddr;
				L2[effAddr % numLines_L2] = effAddr;
				L3[effAddr % numLines_L3] = effAddr;

				hit_L2++;
				miss_L1++;

			}

			else if (L3[effAddr % numLines_L3] == effAddr) {

				L1[effAddr % numLines_L1] = effAddr;
				L2[effAddr % numLines_L2] = effAddr;
				L3[effAddr % numLines_L3] = effAddr;

				hit_L3++;
				miss_L1++;
				miss_L2++;

			}
			else {
				L1[effAddr % numLines_L1] = effAddr;
				L2[effAddr % numLines_L2] = effAddr;
				L3[effAddr % numLines_L3] = effAddr;

				hit_mem++;
				miss_L1++;
				miss_L2++;
				miss_L3++;
			}



		}

		cout << hit_L1 << ' ' << miss_L1 << endl;
		cout << hit_L2 << ' ' << miss_L2 << endl;
		cout << hit_L3 << ' ' << miss_L3 << endl;


		double AMAT_L3 = L3_Speed + (double(miss_L3) / double(miss_L2)) * 100;
		double AMAT_L2 = L2_Speed + (double(miss_L2) / double(miss_L1)) * AMAT_L3;
		double AMAT = L1_Speed + (double(miss_L1) / 1000000) * AMAT_L2;

		cout << AMAT << endl;

		hit_L1 = 0, hit_L2 = 0, miss_L1 = 0, miss_L2 = 0, hit_L3 = 0, miss_L3 = 0, hit_mem = 0;
		addr = 0;

		for (int i = 0; i < numLines_L1; i++) {
			L1[i] = -1;
		}
		for (int i = 0; i < numLines_L2; i++) {
			L2[i] = -1;
		}
		for (int i = 0; i < numLines_L3; i++) {
			L3[i] = -1;
		}


		for (int i = 0; i < 1000000; i++)
		{

			addr = memGen2();

			int effAddr = addr / 64;

			if (L1[effAddr % numLines_L1] == effAddr) {

				L1[effAddr % numLines_L1] = effAddr;
				L2[effAddr % numLines_L2] = effAddr;
				L3[effAddr % numLines_L3] = effAddr;

				hit_L1++;
			}

			else if (L2[effAddr % numLines_L2] == effAddr) {

				L1[effAddr % numLines_L1] = effAddr;
				L2[effAddr % numLines_L2] = effAddr;
				L3[effAddr % numLines_L3] = effAddr;

				hit_L2++;
				miss_L1++;

			}

			else if (L3[effAddr % numLines_L3] == effAddr) {

				L1[effAddr % numLines_L1] = effAddr;
				L2[effAddr % numLines_L2] = effAddr;
				L3[effAddr % numLines_L3] = effAddr;

				hit_L3++;
				miss_L1++;
				miss_L2++;

			}
			else {
				L1[effAddr % numLines_L1] = effAddr;
				L2[effAddr % numLines_L2] = effAddr;
				L3[effAddr % numLines_L3] = effAddr;

				hit_mem++;
				miss_L1++;
				miss_L2++;
				miss_L3++;
			}



		}

		cout << hit_L1 << ' ' << miss_L1 << endl;
		cout << hit_L2 << ' ' << miss_L2 << endl;
		cout << hit_L3 << ' ' << miss_L3 << endl;


		 AMAT_L3 = L3_Speed + (double(miss_L3) / double(miss_L2)) * 100;
		 AMAT_L2 = L2_Speed + (double(miss_L2) / double(miss_L1)) * AMAT_L3;
		 AMAT = L1_Speed + (double(miss_L1) / 1000000) * AMAT_L2;

		cout << AMAT << endl;

		hit_L1 = 0, hit_L2 = 0, miss_L1 = 0, miss_L2 = 0, hit_L3 = 0, miss_L3 = 0, hit_mem = 0;
		addr = 0;

		for (int i = 0; i < numLines_L1; i++) {
			L1[i] = -1;
		}
		for (int i = 0; i < numLines_L2; i++) {
			L2[i] = -1;
		}
		for (int i = 0; i < numLines_L3; i++) {
			L3[i] = -1;
		}


		for (int i = 0; i < 1000000; i++)
		{

			addr = memGen3();

			int effAddr = addr / 64;

			if (L1[effAddr % numLines_L1] == effAddr) {

				L1[effAddr % numLines_L1] = effAddr;
				L2[effAddr % numLines_L2] = effAddr;
				L3[effAddr % numLines_L3] = effAddr;

				hit_L1++;
			}

			else if (L2[effAddr % numLines_L2] == effAddr) {

				L1[effAddr % numLines_L1] = effAddr;
				L2[effAddr % numLines_L2] = effAddr;
				L3[effAddr % numLines_L3] = effAddr;

				hit_L2++;
				miss_L1++;

			}

			else if (L3[effAddr % numLines_L3] == effAddr) {

				L1[effAddr % numLines_L1] = effAddr;
				L2[effAddr % numLines_L2] = effAddr;
				L3[effAddr % numLines_L3] = effAddr;

				hit_L3++;
				miss_L1++;
				miss_L2++;

			}
			else {
				L1[effAddr % numLines_L1] = effAddr;
				L2[effAddr % numLines_L2] = effAddr;
				L3[effAddr % numLines_L3] = effAddr;

				hit_mem++;
				miss_L1++;
				miss_L2++;
				miss_L3++;
			}


		}

		cout << hit_L1 << ' ' << miss_L1 << endl;
		cout << hit_L2 << ' ' << miss_L2 << endl;
		cout << hit_L3 << ' ' << miss_L3 << endl;


		AMAT_L3 = L3_Speed + (double(miss_L3) / double(miss_L2)) * 100;
		AMAT_L2 = L2_Speed + (double(miss_L2) / double(miss_L1)) * AMAT_L3;
		AMAT = L1_Speed + (double(miss_L1) / 1000000) * AMAT_L2;

		cout << AMAT << endl;

		cout << endl;

	}
};

void oneLevelCache_analysis() {

	int SizeOfCache = 256 * 1024;
	int SizeOfBlock = 64;//64

	unsigned int addr;
	int* cache;

	int numberOfLines = SizeOfCache / SizeOfBlock;

	unsigned int effAddr;

	cache = new int[numberOfLines];

	for (int i = 0; i < numberOfLines; i++) {
		cache[i] = -1;
	}

	int miss = 0;
	int hit = 0;
	addr = 0;

	for (int i = 0; i < 1000000; i++)
	{

		addr = memGen1();

		effAddr = addr / (SizeOfBlock);


		if (cache[effAddr % numberOfLines] == effAddr) {
			hit++;
			cache[effAddr % numberOfLines] = effAddr;
		}
		else {
			miss++;
			cache[effAddr % numberOfLines] = effAddr;
		}
	}

	double amat = 11 + double(miss) / 1000000 * 100;
	cout << amat << endl;



	//	cout << "result from 1st Random Generator Funcrion: " << hit / 1000000.0 << endl;

	for (int i = 0; i < numberOfLines; i++) {
		cache[i] = -1;
	}


	miss = 0;
	hit = 0;
	addr = 0;

	for (int i = 0; i < 1000000; i++)
	{

		addr = memGen2();
		effAddr = addr / (SizeOfBlock);


		if (cache[effAddr % numberOfLines] == effAddr) {
			hit++;
			cache[effAddr % numberOfLines] = effAddr;
		}
		else {
			miss++;
			cache[effAddr % numberOfLines] = effAddr;
		}


	}

	amat = 11 + double(miss) / 1000000 * 100;
	cout << amat << endl;

	for (int i = 0; i < numberOfLines; i++) {
		cache[i] = -1;
	}


	miss = 0;
	hit = 0;
	addr = 0;
	for (int i = 0; i < 1000000; i++)
	{

		addr = memGen3();
		effAddr = addr / (SizeOfBlock);


		if (cache[effAddr % numberOfLines] == effAddr) {
			hit++;
			cache[effAddr % numberOfLines] = effAddr;
		}
		else {
			miss++;
			cache[effAddr % numberOfLines] = effAddr;
		}

	}

	amat = 11 + double(miss) / 1000000 * 100;
	cout << amat << endl;

}

//Exp 1
void DirectMapCache(unsigned int SizeOfCache, unsigned int SizeOfBlock) {

	unsigned int addrr;
	int* cache;

	unsigned int numberOfLines = SizeOfCache / SizeOfBlock;

	unsigned int effAddr;

	cache = new int[numberOfLines];

	for (int i = 0; i < numberOfLines; i++) {
		cache[i] = -1;
	}

	int miss = 0;
	int hit = 0;
	unsigned int addr = 0;
	addrr = 0;

	for (int i = 0; i < 1000000; i++)
	{

		addrr = memGen1();

		effAddr = addrr / (SizeOfBlock);


		if (cache[effAddr % numberOfLines] == effAddr) {
			hit++;
			cache[effAddr % numberOfLines] = effAddr;
		}
		else {
			miss++;
			cache[effAddr % numberOfLines] = effAddr;
		}


	}

	cout << "result from 1st Random Generator Funcrion: " << hit / 1000000.0 << endl;

	for (int i = 0; i < numberOfLines; i++) {
		cache[i] = -1;
	}


	miss = 0;
	hit = 0;
	addr = 0;
	addrr = 0;

	for (int i = 0; i < 1000000; i++)
	{

		addrr = memGen2();
		effAddr = addrr / (SizeOfBlock);


		if (cache[effAddr % numberOfLines] == effAddr) {
			hit++;
			cache[effAddr % numberOfLines] = effAddr;
		}
		else {
			miss++;
			cache[effAddr % numberOfLines] = effAddr;
		}


	}

	cout << "result from 2nd Random Generator Funcrion: " << hit / 1000000.0 << endl;

	for (int i = 0; i < numberOfLines; i++) {
		cache[i] = -1;
	}

	miss = 0;
	hit = 0;
	addrr = 0;
	addr = 0;
	for (int i = 0; i < 1000000; i++)
	{

		addrr = memGen3();
		effAddr = addrr / (SizeOfBlock);


		if (cache[effAddr % numberOfLines] == effAddr) {
			hit++;
			cache[effAddr % numberOfLines] = effAddr;
		}
		else {
			miss++;
			cache[effAddr % numberOfLines] = effAddr;
		}

	}

	cout << "result from 3rd Random Generator Funcrion: " << hit / 1000000.0 << endl;

}
//Exp 5
void n_way_set_associative(int n) {

	int cacheSize = 16 * 1024;
	int blockSize = 8;


	int numberOfSets = (cacheSize / blockSize) / n;

	int* pos = new int[numberOfSets];

	int** cache = new int* [numberOfSets];

	for (int i = 0; i < numberOfSets; i++) {
		cache[i] = new int[n];
	}

	for (int i = 0; i < numberOfSets; i++) {
		pos[i] = 0;
	}

	int miss = 0;
	int hit = 0;
	int effAddr;
	int order;

	bool found = 0;

	unsigned int addr = 0;


	for (int x = 0; x < 3; x++) {

		for (int i = 0; i < numberOfSets; i++) {
			pos[i] = 0;
		}

		miss = 0;
		hit = 0;

		for (int i = 0; i < numberOfSets; i++) {
			for (int j = 0; j < n; j++) {

				cache[i][j] = -1;
			}
		}



		for (int j = 0; j < 1000000; j++)
		{

			switch (x) {
			case 0:
				addr = memGen1();
				break;
			case 1:
				addr = memGen2();
				break;
			default:
				addr = memGen3();
			}

			effAddr = addr / (blockSize);


			for (int i = 0; i < n; i++) {
				if (cache[effAddr % numberOfSets][i] == effAddr)
				{
					found = true;
					hit++;
					break;

				}
			}

			if (!found) {
				miss++;
				order = pos[effAddr % numberOfSets] % n;
				cache[effAddr % numberOfSets][order] = effAddr;
				pos[effAddr % numberOfSets]++;
			}

			found = 0;

		}

		cout << "gen " << x << " " << n << " - way : miss =  " << miss << ", hits = " << hit << " miss rate: " << double(miss) / 1000000 << endl;

	}

}
//Exp 3

int recent = 0;

cacheResType searchinFullAss( int add,  int* cache, int size)
{
	for (int i = 0; i < size; i++) {
		if (cache[i] == -1) break;
		else if (cache[i] == add)
		{
			recent = i;
			return HIT;
		}
	}
	return MISS;
}
void FullAssociative(const int blocksize, int cachesize)
{
	int lines = cachesize / blocksize;
	int  crntblock = 0;
	 int* cache = new  int[lines];
	unsigned int memadd, blockadd;
	for (int i = 0; i < lines; i++) cache[i] = -1;
	cacheResType x;
	int hits = 0, miss = 0;
	for (int i = 0; i < 1000000; i++)
	{
		memadd = memGen1();
		blockadd = memadd / blocksize;
		x = searchinFullAss(blockadd, cache, lines);
		if (x) { hits++; }
		else
		{
			miss++;
			if (crntblock < lines) {
				cache[crntblock++] = blockadd; recent = crntblock;
			}
			else {

				cache[recent] = blockadd;
			}
			 
		}

	}
	cout << "Full Associative Cache with cache size of " << cachesize << " with MemGen1() has a hit ratio of: " << hits / 1000000.0 << endl;
	hits = 0; miss = 0, crntblock = 0;
	for (int i = 0; i < lines; i++) cache[i] = -1;
	for (int i = 0; i < 1000000; i++)
	{
		memadd = memGen2();
		blockadd = memadd / blocksize;
		x = searchinFullAss(blockadd, cache, lines);
		if (x) { hits++; }
		else
		{
			miss++;
			if (crntblock < lines) {
				cache[crntblock++] = blockadd; recent = crntblock;
			}
			else {

				cache[recent] = blockadd;
			}

		}

	}
	cout << "Full Associative Cache with cache size of " << cachesize << " with MemGen2() has a hit ratio of: " << hits / 1000000.0 << endl;
	hits = 0; miss = 0, crntblock=0;
	for (int i = 0; i < lines; i++) cache[i] = -1;
	for (int i = 0; i < 1000000; i++)
	{
		memadd = memGen3();
		blockadd = memadd / blocksize;
		x = searchinFullAss(blockadd, cache, lines);
		if (x) { hits++; }
		else
		{
			miss++;
			if (crntblock < lines) {
				cache[crntblock++] = blockadd; recent = crntblock;
			}
			else {

				cache[recent] = blockadd;
			}

		}

	}
	cout << "Full Associative Cache with cache size of " << cachesize << " with MemGen3() has a hit ratio of: " << hits / 1000000.0 << endl;

}
//Exp 4
cacheResType searchinFullAss2( int add,  int* cache, int size, unsigned int*A)
{
	for (int i = 0; i < size; i++)
	{
		if (cache[i] == -1) break;
		else if (cache[i] == add)
		{
			A[i]++;
			return HIT;
		}
	}
	return MISS;
}
void replaceRandomly( int* cache, int size, int add)
{
	int randblock = rand() % size;
	cache[randblock] = add;
	
}
int  getmin(unsigned int* A, int size)
{
	int m = A[0];
	for (int i = 1; i < size; i++) if (A[i] < m)m = A[i];
	return m;
}
void choosebest(bool flag,  int* cache, unsigned int* uses, int &crntblock, int lines, int blockadd)
{
	int m;
	if (flag)
	{
		if (crntblock < lines) {//LRU
			cache[crntblock] = blockadd; uses[crntblock++]++;
		}
		else {
			m = getmin(uses, lines);
			cache[m] = blockadd;
		}
	}
	else
	{
		cache[crntblock % lines] = blockadd;//LFU >> start over
		crntblock++;
	}

}


void cacheWithReplacements(const int blocksize, const int cachesize)
{
	int lines = cachesize / blocksize;
	int hits = 0, miss = 0, crntblock=0;
	 int* cache = new  int[lines];
	 for (int i = 0; i < lines; i++) cache[i] = -1;
	
	cacheResType x;
	 int memadd, blockadd;
	//random replacement
	for (int i = 0; i < 1000000; i++)
	{
		memadd = memGen1();
		blockadd = memadd / blocksize;
		x = searchinFullAss(blockadd, cache, lines);
		if (x) { hits++; }
		else
		{
			miss++;
			if (crntblock < lines) {
				cache[crntblock++] = blockadd; 
			}
			else {

				replaceRandomly(cache, lines, blockadd);
			}

			
		}

	}
cout<< "Full Associative Cache with cache size of " << cachesize << " with MemGen1() and random raplacement has a hit ratio of: " << hits / 1000000.0 << endl;
hits = 0; miss = 0, crntblock=0;
for (int i = 0; i < lines; i++) cache[i] = -1;
for (int i = 0; i < 1000000; i++)
{
	memadd = memGen2();
	blockadd = memadd / blocksize;
	x = searchinFullAss(blockadd, cache, lines);
	if (x) { hits++; }
	else
	{
		miss++;
		if (crntblock < lines) {
			cache[crntblock++] = blockadd; 
		}
		else {

			replaceRandomly(cache, lines, blockadd);
		}

	}

}
cout << "Full Associative Cache with cache size of " << cachesize << " with MemGen2() and random raplacement has a hit ratio of: " << hits / 1000000.0 << endl;
hits = 0; miss = 0,crntblock=0;
for (int i = 0; i < lines; i++) cache[i] = -1;
for (int i = 0; i < 1000000; i++)
{
	memadd = memGen3();
	blockadd = memadd / blocksize;
	x = searchinFullAss(blockadd, cache, lines);
	if (x) { hits++; }
	else
	{
		miss++;
		if (crntblock < lines) {
			cache[crntblock++] = blockadd; 
		}
		else {

			replaceRandomly(cache, lines, blockadd);
		}

	}

}
cout << "Full Associative Cache with cache size of " << cachesize << " with MemGen3() and random raplacement has a hit ratio of: " << hits / 1000000.0 << endl;


//LFU policy
hits = 0, crntblock = 0, miss = 0;
for (int i = 0; i < lines; i++) cache[i] = -1;
unsigned int* uses = new unsigned int[lines];
for (int i = 0; i < lines; i++) uses[i] = 0;

int m;
for (int i = 0; i < 1000000; i++)
{
	memadd = memGen1();
	blockadd = memadd / blocksize;
	x = searchinFullAss(blockadd, cache, lines);
	if (x) { hits++;  }
	else
	{
		miss++;
		if (crntblock < lines) {
			cache[crntblock] = blockadd; uses[crntblock++]++;
		}
		else {
			m = getmin(uses, lines);
			cache[m] = blockadd;
		}

		
	}

}
cout << "Full Associative Cache with cache size of " << cachesize << " with MemGen1() and LFU raplacement has a hit ratio of: " << hits / 1000000.0 << endl;
hits = 0, crntblock = 0, miss = 0;
for (int i = 0; i < lines; i++) cache[i] = -1;
for (int i = 0; i < lines; i++) uses[i] = 0;

for (int i = 0; i < 1000000; i++)
{
	memadd = memGen2();
	blockadd = memadd / blocksize;
	x = searchinFullAss(blockadd, cache, lines);
	if (x) { hits++; }
	else
	{
		miss++;
		if (crntblock < lines) {
			cache[crntblock] = blockadd; uses[crntblock++]++;
		}
		else {
			m = getmin(uses, lines);
			cache[m] = blockadd;
		}
	}

}

cout << "Full Associative Cache with cache size of " << cachesize << " with MemGen2() and LFU raplacement has a hit ratio of: " << hits / 1000000.0 << endl;
hits = 0, crntblock = 0, miss = 0;
for (int i = 0; i < lines; i++) cache[i] = -1;
for (int i = 0; i < lines; i++) uses[i] = 0;

for (int i = 0; i < 1000000; i++)
{
	memadd = memGen3();
	blockadd = memadd / blocksize;
	x = searchinFullAss(blockadd, cache, lines);
	if (x) { hits++; }
	else
	{
		miss++;
		if (crntblock < lines) {
			cache[crntblock] = blockadd; uses[crntblock++]++;
		}
		else {
			m = getmin(uses, lines);
			cache[m] = blockadd;
		}
	}
}
cout << "Full Associative Cache with cache size of " << cachesize << " with MemGen3() and LFU raplacement has a hit ratio of: " << hits / 1000000.0 << endl;

//LRU Policy
hits = 0, miss=0, crntblock=0;
for (int i = 0; i < lines; i++) cache[i] = -1;
for (int i = 0; i < 1000000; i++)
{
	memadd = memGen1();
	blockadd = memadd / blocksize;
	x = searchinFullAss(blockadd, cache, lines);
	if (x) { hits++; }
	else
	{
		miss++;
		cache[crntblock % lines] = blockadd;//LFU >> start over
		crntblock++;
	}
}
cout << "Full Associative Cache with cache size of " << cachesize << " with MemGen1() and LRU raplacement has a hit ratio of: " << hits / 1000000.0 << endl;
hits = 0, miss = 0, crntblock = 0;
for (int i = 0; i < lines; i++) cache[i] = -1;
for (int i = 0; i < 1000000; i++)
{
	memadd = memGen2();
	blockadd = memadd / blocksize;
	x = searchinFullAss(blockadd, cache, lines);
	if (x) { hits++; }
	else
	{
		miss++;
		cache[crntblock % lines] = blockadd;//LFU >> start over
		crntblock++;
	}
}
cout << "Full Associative Cache with cache size of " << cachesize << " with MemGen2() and LRU raplacement has a hit ratio of: " << hits / 1000000.0 << endl;
hits = 0, miss = 0, crntblock = 0;
for (int i = 0; i < lines; i++) cache[i] = -1;
for (int i = 0; i < 1000000; i++)
{
	memadd = memGen3();
	blockadd = memadd / blocksize;
	x = searchinFullAss(blockadd, cache, lines);
	if (x) { hits++; }
	else
	{
		miss++;
		cache[crntblock % lines] = blockadd;//LFU >> start over
		crntblock++;
	}
}
cout << "Full Associative Cache with cache size of " << cachesize << " with MemGen3() and LRU raplacement has a hit ratio of: " << hits / 1000000.0 << endl;


//Adaptive LRU/LFU.
//use LRU first then if misses exceeds 50K for example, switch 
int sindx=0;// switch indicator we don't want to reset the miss rate.
hits = 0, crntblock = 0, miss = 0;
bool sw = 0;
for (int i = 0; i < lines; i++) cache[i] = -1;
for (int i = 0; i < 1000000; i++)
{
	memadd = memGen1();
	blockadd = memadd / blocksize;
	x = searchinFullAss(blockadd, cache, lines);
	if (x) { hits++; }
	else
	{
		miss++;
		sindx++;
		if (sindx >50000) {
			sw = !sw; sindx = 0;
		}
		choosebest(sw, cache, uses, crntblock, lines, blockadd);//switch based on the flag sw
	}
}
cout << "Full Associative Cache with cache size of " << cachesize << " with MemGen1() and Adaptive LRU/LFU. raplacement has a hit ratio of: " << hits / 1000000.0 << endl;
 sindx = 0;// switch indicator we don't want to reset the miss rate.
hits = 0, crntblock = 0, miss = 0;
for (int i = 0; i < lines; i++) cache[i] = -1;
 sw = 0;
 for (int i = 0; i < 1000000; i++)
 {
	 memadd = memGen2();
	 blockadd = memadd / blocksize;
	 x = searchinFullAss(blockadd, cache, lines);
	 if (x) { hits++; }
	 else
	 {
		 miss++;
		 sindx++;
		 if (sindx > 50000) {
			 sw = !sw; sindx = 0;
		 }
		 choosebest(sw, cache, uses, crntblock, lines, blockadd);//switch based on the flag sw
	 }
 }
 cout << "Full Associative Cache with cache size of " << cachesize << " with MemGen2() and Adaptive LRU/LFU. raplacement has a hit ratio of: " << hits / 1000000.0 << endl;

 sindx = 0;// switch indicator we don't want to reset the miss rate.
 hits = 0, crntblock = 0, miss = 0;
 for (int i = 0; i < lines; i++) cache[i] = -1;
 sw = 0;
 for (int i = 0; i < 1000000; i++)
 {
	 memadd = memGen3();
	 blockadd = memadd / blocksize;
	 x = searchinFullAss(blockadd, cache, lines);
	 if (x) { hits++; }
	 else
	 {
		 miss++;
		 sindx++;
		 if (sindx > 50000) {
			 sw = !sw; sindx = 0;
		 }
		 choosebest(sw, cache, uses, crntblock, lines, blockadd);
	 }
 }
 cout << "Full Associative Cache with cache size of " << cachesize << " with MemGen3() and Adaptive LRU/LFU. raplacement has a hit ratio of: " << hits / 1000000.0 << endl;



}
//Exp 6
class n_way_two_Level {

private:

	int level1[64][8];		// size 32kb
	int level2[512][8];		// size 256 kb

	int pos_level1[64];
	int pos_level2[512];

	int L1_Speed = 4;
	int L2_Speed = 11;
	int Mem_speed = 100;



public:
	n_way_two_Level() {

		for (int i = 0; i < 64; i++) {
			for (int j = 0; j < 8; j++) {
				level1[i][j] = -1;
			}
		}
		for (int i = 0; i < 64; i++) {
			pos_level1[i] = 0;
		}
		for (int i = 0; i < 512; i++) {
			for (int j = 0; j < 8; j++) {
				level2[i][j] = -1;
			}
		}
		for (int i = 0; i < 512; i++) {
			pos_level2[i] = 0;
		}

	}


	void Analysis() {


		int hit_L1 = 0, hit_L2 = 0, miss_L1 = 0, miss_L2 = 0, hit_mem = 0;
		int effAddr;
		int addr;
		int order_l1;
		int order_l2;

		bool found_l1 = 0;
		bool found_l2 = 0;

		for (int x = 0; x < 3; x++) {

			for (int i = 0; i < 64; i++) {
				for (int j = 0; j < 8; j++) {
					level1[i][j] = -1;
				}
			}
			for (int i = 0; i < 64; i++) {
				pos_level1[i] = 0;
			}
			for (int i = 0; i < 512; i++) {
				for (int j = 0; j < 8; j++) {
					level2[i][j] = -1;
				}
			}
			for (int i = 0; i < 512; i++) {
				pos_level2[i] = 0;
			}

			hit_L1 = 0; hit_L2 = 0; miss_L1 = 0; miss_L2 = 0; hit_mem = 0;

			bool found_l1 = 0;
			bool found_l2 = 0;


			for (int j = 0; j < 1000000; j++)

			{
				switch (x) {
				case 0:
					addr = memGen1();
					break;
				case 1:
					addr = memGen2();
					break;
				default:
					addr = memGen3();
				}


				effAddr = addr / 64;

				for (int i = 0; i < 8; i++) {
					if (level1[effAddr % 64][i] == effAddr)
					{
						found_l1 = true;
						hit_L1++;

						order_l1 = pos_level1[effAddr % 64] % 8;
						order_l2 = pos_level2[effAddr % 512] % 8;

						for (int k = 0; k < 8; k++) {
							if (level2[effAddr % 512][k] == effAddr) found_l2 = true;
						}

						if (!found_l2) {
							level2[effAddr % 512][order_l2] = effAddr;
							pos_level2[effAddr % 512]++;
						}

						break;

					}
				}

				if (!found_l1) {
					for (int i = 0; i < 8; i++) {

						if (level2[effAddr % 512][i] == effAddr)
						{
							found_l2 = true;

							order_l1 = pos_level1[effAddr % 64] % 8;
							order_l2 = pos_level2[effAddr % 512] % 8;

							level1[effAddr % 64][order_l1] = effAddr;

							pos_level1[effAddr % 64]++;

							hit_L2++;
							miss_L1++;

							break;
						}
					}

				}

				if (!found_l2) {

					order_l1 = pos_level1[effAddr % 64] % 8;
					order_l2 = pos_level2[effAddr % 512] % 8;
					level1[effAddr % 64][order_l1] = effAddr;
					level2[effAddr % 512][order_l2] = effAddr;
					pos_level1[effAddr % 64]++;
					pos_level2[effAddr % 512]++;

					miss_L2++;
					miss_L1++;
				}

				found_l1 = 0;
				found_l2 = 0;

			}

			double AMAT_L2 = L2_Speed + (double(miss_L2) / double(miss_L1)) * Mem_speed;
			double AMAT = L1_Speed + (double(miss_L1) / 1000000) * AMAT_L2;
			cout << miss_L2 << " " << miss_L1 << endl;
			cout << x + 1 << ' ' << AMAT << endl;
		}


	}

};

void n_way_set_associative_OneLevel(int n) {

	int cacheSize = 256 * 1024;
	int blockSize = 64;


	int numberOfSets = (cacheSize / blockSize) / n;

	int* pos = new int[numberOfSets];

	int** cache = new int* [numberOfSets];

	for (int i = 0; i < numberOfSets; i++) {
		cache[i] = new int[n];
	}

	for (int i = 0; i < numberOfSets; i++) {
		pos[i] = 0;
	}

	int miss = 0;
	int hit = 0;
	int effAddr;
	int order;

	bool found = 0;

	unsigned int addr = 0;


	for (int x = 0; x < 3; x++) {

		for (int i = 0; i < numberOfSets; i++) {
			pos[i] = 0;
		}

		miss = 0;
		hit = 0;

		for (int i = 0; i < numberOfSets; i++) {
			for (int j = 0; j < n; j++) {

				cache[i][j] = -1;
			}
		}



		for (int j = 0; j < 1000000; j++)
		{

			switch (x) {
			case 0:
				addr = memGen1();
				break;
			case 1:
				addr = memGen2();
				break;
			default:
				addr = memGen3();
			}

			effAddr = addr / (blockSize);


			for (int i = 0; i < n; i++) {
				if (cache[effAddr % numberOfSets][i] == effAddr)
				{
					found = true;
					hit++;
					break;

				}
			}

			if (!found) {
				miss++;
				order = pos[effAddr % numberOfSets] % n;
				cache[effAddr % numberOfSets][order] = effAddr;
				pos[effAddr % numberOfSets]++;
			}

			found = 0;

		}

		double amat = 11 + double(miss) / 1000000 * 100;
		cout<<"MemGen# "<< x<<" " << amat << endl;
	}

}
class n_way_three_Level {

private:

	int level1[64][8];		// size 32kb
	int level2[512][8];		// size 256 kb
	int level3[8192][16];


	int pos_level1[64];
	int pos_level2[512];
	int pos_level3[8192];

	int L1_Speed = 4;
	int L2_Speed = 11;
	int L3_Speed = 30;
	int Mem_speed = 100;



public:
	n_way_three_Level() {

		for (int i = 0; i < 64; i++) {
			for (int j = 0; j < 8; j++) {
				level1[i][j] = -1;
			}
		}
		for (int i = 0; i < 64; i++) {
			pos_level1[i] = 0;
		}
		for (int i = 0; i < 512; i++) {
			for (int j = 0; j < 8; j++) {
				level2[i][j] = -1;
			}
		}
		for (int i = 0; i < 512; i++) {
			pos_level2[i] = 0;
		}
		for (int i = 0; i < 8192; i++) {
			for (int j = 0; j < 16; j++) {
				level3[i][j] = -1;
			}
		}
		for (int i = 0; i < 8192; i++) {
			pos_level3[i] = 0;
		}

	}


	void Analysis() {


		int hit_L1 = 0, hit_L2 = 0, miss_L1 = 0, miss_L2 = 0, hit_mem = 0, hit_L3 = 0, miss_L3 = 0;
		int effAddr;
		int addr;
		int order_l1;
		int order_l2;
		int order_l3;

		bool found_l1 = 0;
		bool found_l2 = 0;
		bool found_l3 = 3;

		for (int x = 0; x < 3; x++) {


			for (int i = 0; i < 64; i++) {
				for (int j = 0; j < 8; j++) {
					level1[i][j] = -1;
				}
			}
			for (int i = 0; i < 64; i++) {
				pos_level1[i] = 0;
			}
			for (int i = 0; i < 512; i++) {
				for (int j = 0; j < 8; j++) {
					level2[i][j] = -1;
				}
			}
			for (int i = 0; i < 512; i++) {
				pos_level2[i] = 0;
			}
			for (int i = 0; i < 8192; i++) {
				for (int j = 0; j < 16; j++) {
					level3[i][j] = -1;
				}
			}
			for (int i = 0; i < 8192; i++) {
				pos_level3[i] = 0;
			}


			hit_L1 = 0; hit_L2 = 0; miss_L1 = 0; miss_L2 = 0; hit_mem = 0; hit_L3 = 0; miss_L3 = 0;
			found_l1 = 0;
			found_l2 = 0;
			found_l3 = 0;


			for (int j = 0; j < 1000000; j++)

			{
				switch (x) {
				case 0:
					addr = memGen1();
					break;
				case 1:
					addr = memGen2();
					break;
				default:
					addr = memGen3();
				}


				effAddr = addr / 64;


				for (int i = 0; i < 8; i++) {

					if (level1[effAddr % 64][i] == effAddr) {

						found_l1 = true;
						found_l2 = true;
						found_l3 = true;
						hit_L1++;

						order_l1 = pos_level1[effAddr % 64] % 8;
						order_l2 = pos_level2[effAddr % 512] % 8;
						order_l3 = pos_level3[effAddr % 8192] % 16;

						for (int k = 0; k < 8; k++) {
							if (level2[effAddr % 512][k] == effAddr) found_l2 = true;
						}

						if (!found_l2) {
							level2[effAddr % 512][order_l2] = effAddr;
							pos_level2[effAddr % 512]++;
						}

						for (int k = 0; k < 16; k++) {
							if (level3[effAddr % 8192][k] == effAddr) found_l3 = true;
						}

						if (!found_l3) {
							level3[effAddr % 8192][order_l3] = effAddr;
							pos_level3[effAddr % 8192]++;
						}

						break;
					}

				}

				if (!found_l1) {

					for (int i = 0; i < 8; i++) {

						if (level2[effAddr % 512][i] == effAddr) {

							found_l2 = true;
							found_l3 = true;
							order_l1 = pos_level1[effAddr % 64] % 8;
							order_l2 = pos_level2[effAddr % 512] % 8;
							order_l3 = pos_level3[effAddr % 8192] % 16;

							level1[effAddr % 64][order_l1] = effAddr;
							pos_level1[effAddr % 64]++;

							for (int k = 0; k < 16; k++) {
								if (level3[effAddr % 8192][k] == effAddr) found_l3 = true;
							}

							if (!found_l3) {
								level3[effAddr % 8192][order_l3] = effAddr;
								pos_level3[effAddr % 8192]++;
							}

							hit_L2++;
							miss_L1++;

							break;
						}


					}

				}

				if (!found_l2) {

					for (int i = 0; i < 16; i++) {

						if (level3[effAddr % 8192][i] == effAddr) {

							found_l3 = true;

							order_l1 = pos_level1[effAddr % 64] % 8;
							order_l2 = pos_level2[effAddr % 512] % 8;
							order_l3 = pos_level3[effAddr % 8192] % 16;

							level1[effAddr % 64][order_l1] = effAddr;
							level2[effAddr % 512][order_l2] = effAddr;

							pos_level1[effAddr % 64]++;
							pos_level2[effAddr % 512]++;


							miss_L2++;
							miss_L1++;

							break;

						}

					}


				}

				if (!found_l3) {

					order_l1 = pos_level1[effAddr % 64] % 8;
					order_l2 = pos_level2[effAddr % 512] % 8;
					order_l3 = pos_level3[effAddr % 8192] % 16;

					level1[effAddr % 64][order_l1] = effAddr;
					level2[effAddr % 512][order_l2] = effAddr;
					level3[effAddr % 8192][order_l3] = effAddr;

					pos_level1[effAddr % 64]++;
					pos_level2[effAddr % 512]++;
					pos_level3[effAddr % 8192]++;

					miss_L3++;
					miss_L2++;
					miss_L1++;

				}

				found_l3 = 0;
				found_l2 = 0;
				found_l1 = 0;


			}

			double AMAT_L3 = L3_Speed + (double(miss_L3) / double(miss_L2)) * 100;
			double AMAT_L2 = L2_Speed + (double(miss_L2) / double(miss_L1) )* AMAT_L3;
			double AMAT = L1_Speed + (double(miss_L1) / 1000000) * AMAT_L2;
			cout << miss_L1 << " " << miss_L2 << " " << miss_L3 << endl;
			cout << x + 1 << ' ' << AMAT << endl;


		}
	}


};

int main() {
	srand(time(NULL));
	////Exp 2
	oneLevelCache_analysis();
	twoLevelCache C2;
	C2.Analysis();
	threeLevelCache C3;
	C3.Analysis();
	//Exp 1
	for (int i = 4; i <= 128; i *= 2) {
		cout << i << endl;
		DirectMapCache(64 * 1024, i);
	}
	testing full associative cache
	
	cout << "Exp 3: ................................................................." << endl;
	int size = 4 * 1024; const int blocksize = 16;
	while (size <= 64 * 1024)
	{
		FullAssociative(blocksize, size);
		size *= 2;
	}
	//Exp 4
	cout << "Exp 4........................................................................" << endl;
	cacheWithReplacements(32, 256 * 1024);
	
	
	
	//Exp 5
	for (int i = 2; i <= 16; i *= 2) {
		cout << i << endl;
		n_way_set_associative(i);
	}*/
	//Exp 6
	n_way_set_associative_OneLevel(8);
	n_way_two_Level nC2;
	nC2.Analysis();
	n_way_three_Level nC3;
	nC3.Analysis();

	system("pause");
	return 0;
}


