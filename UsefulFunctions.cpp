/*
 *  UsefulFunctions.cpp
 *  Untitled
 *
 *  Created by Steven Christe on 7/22/09.
 *  Copyright 2009 FOXSI. All rights reserved.
 *
 */

#include "UsefulFunctions.h"

string convertBase(unsigned long v, long base)
{
	string digits = "0123456789abcdef";
	string result;
	if((base < 2) || (base > 32)) {
		result = "Error: base out of range.";
	}
	else {
		do {
			result = digits[v % base] + result;
			v /= base;
		}
		while(v);
	}
	return result;
}

unsigned getbits(unsigned x, int p, int n)
{
	// This function extracts n bits, starting at position p, from integer x.
	// The most common use is n=1 to extract a single bit at position p.
	// p=0 refers to the rightmost (LSB) bit.
	// The full description is at http://www.java-samples.com/showtutorial.php?tutorialid=500
	
	return (x >> (p+1-n)) & ~(~0 << n);
	
}

unsigned reversebits(unsigned x, int n)
{
	// This function reverses the bit order of an integer and returns it.  Only the n number of LSB are included;
	// all other bits are zero.
	
	// example for 5 bits; the code does:
	// getbits(x,4,1)*1 + getbits(x,3,1)*2 + getbits(x,2,1)*4 + getbits(x,1,1)*8 + getbits(x,0,1)*16
	
	unsigned y = 0;
	
	for(int i=0; i<n; i++){
		
		y = y + getbits(x, n-i-1, 1)*pow(2,i);
		//		cout << y << '\t';  // debug
		
	}
	
	return y;
	
}

unsigned int median(unsigned int *a, int n)
{
	float temp;
	int i,j;
	for(i=0;i<n;i++)
		for(j=i+1;j<n;j++)
		{
			if(a[i]>a[j])
			{
				temp=a[j];
				a[j]=a[i];
				a[i]=temp;
			}
		}
	if(n%2==0)
		return (a[n/2]+a[n/2-1])/2;
	else
		return a[n/2];
}