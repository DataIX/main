
/*
 *
 * 1 -> 16 byte 9090 PWL Generator (numeric)
 *
 * $Id$
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <unistd.h>

int main(void)
{
  int cf,ce,cd,cc,cb,ca,c9,c8,c7,c6,c5,c4,c3,c2,c1,c0;
  int start=0,end=9;
  int length=6;

  /* Deprecated for now.
  printf("Password Length (1-16): ");
  scanf("%d[1-16]", &length);
  */

  if(length==16)
    for(c0=end; c0>=start; c0--)
      for(c1=start; c1<=end; c1++)
	for(c2=end; c2>=start; c2--)
	  for(c3=start; c3<=end; c3++)
	    for(c4=end; c4>=start; c4--)
	      for(c5=start; c5<=end; c5++)
		for(c6=end; c6>=start; c6--)
		  for(c7=start; c7<=end; c7++)
		    for(c8=end; c8>=start; c8--)
		      for(c9=start; c9<=end; c9++)
                        for(ca=end; ca>=start; ca--)
			  for(cb=start; cb<=end; cb++)
			    for(cc=end; cc>=start; cc--)
			      for(cd=start; cd<=end; cd++)
				for(ce=end; ce>=start; ce--)
				  for(cf=start; cf<=end; cf++)
				    printf("%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d\n",
					   cf,ce,cd,cc,cb,ca,c9,c8,c7,c6,c5,c4,c3,c2,c1,c0);

  if(length==15)
    for(c0=end; c0>=start; c0--)
      for(c1=start; c1<=end; c1++)
	for(c2=end; c2>=start; c2--)
	  for(c3=start; c3<=end; c3++)
	    for(c4=end; c4>=start; c4--)
	      for(c5=start; c5<=end; c5++)
		for(c6=end; c6>=start; c6--)
		  for(c7=start; c7<=end; c7++)
		    for(c8=end; c8>=start; c8--)
		      for(c9=start; c9<=end; c9++)
                        for(ca=end; ca>=start; ca--)
			  for(cb=start; cb<=end; cb++)
			    for(cc=end; cc>=start; cc--)
			      for(cd=start; cd<=end; cd++)
				for(ce=end; ce>=start; ce--)
				  printf("%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d\n",
					 ce,cd,cc,cb,ca,c9,c8,c7,c6,c5,c4,c3,c2,c1,c0);

  if(length==14)
    for(c0=end; c0>=start; c0--)
      for(c1=start; c1<=end; c1++)
	for(c2=end; c2>=start; c2--)
	  for(c3=start; c3<=end; c3++)
	    for(c4=end; c4>=start; c4--)
	      for(c5=start; c5<=end; c5++)
		for(c6=end; c6>=start; c6--)
		  for(c7=start; c7<=end; c7++)
		    for(c8=end; c8>=start; c8--)
		      for(c9=start; c9<=end; c9++)
                        for(ca=end; ca>=start; ca--)
			  for(cb=start; cb<=end; cb++)
			    for(cc=end; cc>=start; cc--)
			      for(cd=start; cd<=end; cd++)
                                printf("%d%d%d%d%d%d%d%d%d%d%d%d%d%d\n",
				       cd,cc,cb,ca,c9,c8,c7,c6,c5,c4,c3,c2,c1,c0);

  if(length==13)
    for(c0=end; c0>=start; c0--)
      for(c1=start; c1<=end; c1++)
	for(c2=end; c2>=start; c2--)
	  for(c3=start; c3<=end; c3++)
	    for(c4=end; c4>=start; c4--)
	      for(c5=start; c5<=end; c5++)
		for(c6=end; c6>=start; c6--)
		  for(c7=start; c7<=end; c7++)
		    for(c8=end; c8>=start; c8--)
		      for(c9=start; c9<=end; c9++)
                        for(ca=end; ca>=start; ca--)
			  for(cb=start; cb<=end; cb++)
			    for(cc=end; cc>=start; cc--)
			      printf("%d%d%d%d%d%d%d%d%d%d%d%d%d\n",
				     cc,cb,ca,c9,c8,c7,c6,c5,c4,c3,c2,c1,c0);

  if(length==12)
    for(c0=end; c0>=start; c0--)
      for(c1=start; c1<=end; c1++)
	for(c2=end; c2>=start; c2--)
	  for(c3=start; c3<=end; c3++)
	    for(c4=end; c4>=start; c4--)
	      for(c5=start; c5<=end; c5++)
		for(c6=end; c6>=start; c6--)
		  for(c7=start; c7<=end; c7++)
		    for(c8=end; c8>=start; c8--)
		      for(c9=start; c9<=end; c9++)
                        for(ca=end; ca>=start; ca--)
			  for(cb=start; cb<=end; cb++)
			    printf("%d%d%d%d%d%d%d%d%d%d%d%d\n",
				   cb,ca,c9,c8,c7,c6,c5,c4,c3,c2,c1,c0);

  if(length==11)
    for(c0=end; c0>=start; c0--)
      for(c1=start; c1<=end; c1++)
	for(c2=end; c2>=start; c2--)
	  for(c3=start; c3<=end; c3++)
	    for(c4=end; c4>=start; c4--)
	      for(c5=start; c5<=end; c5++)
		for(c6=end; c6>=start; c6--)
		  for(c7=start; c7<=end; c7++)
		    for(c8=end; c8>=start; c8--)
		      for(c9=start; c9<=end; c9++)
                        for(ca=end; ca>=start; ca--)
			  printf("%d%d%d%d%d%d%d%d%d%d%d\n",
				 ca,c9,c8,c7,c6,c5,c4,c3,c2,c1,c0);

  if(length==10)
    for(c0=end; c0>=start; c0--)
      for(c1=start; c1<=end; c1++)
	for(c2=end; c2>=start; c2--)
	  for(c3=start; c3<=end; c3++)
	    for(c4=end; c4>=start; c4--)
	      for(c5=start; c5<=end; c5++)
		for(c6=end; c6>=start; c6--)
		  for(c7=start; c7<=end; c7++)
		    for(c8=end; c8>=start; c8--)
		      for(c9=start; c9<=end; c9++)
			printf("%d%d%d%d%d%d%d%d%d%d\n",
			       c9,c8,c7,c6,c5,c4,c3,c2,c1,c0);

  if(length==9)
    for(c0=end; c0>=start; c0--)
      for(c1=start; c1<=end; c1++)
	for(c2=end; c2>=start; c2--)
	  for(c3=start; c3<=end; c3++)
	    for(c4=end; c4>=start; c4--)
	      for(c5=start; c5<=end; c5++)
		for(c6=end; c6>=start; c6--)
		  for(c7=start; c7<=end; c7++)
		    for(c8=end; c8>=start; c8--)
		      printf("%d%d%d%d%d%d%d%d%d\n",
			     c8,c7,c6,c5,c4,c3,c2,c1,c0);

  if(length==8)
    for(c0=end; c0>=start; c0--)
      for(c1=start; c1<=end; c1++)
	for(c2=end; c2>=start; c2--)
	  for(c3=start; c3<=end; c3++)
	    for(c4=end; c4>=start; c4--)
	      for(c5=start; c5<=end; c5++)
		for(c6=end; c6>=start; c6--)
		  for(c7=start; c7<=end; c7++)
		    printf("%d%d%d%d%d%d%d%d\n",
			   c7,c6,c5,c4,c3,c2,c1,c0);

  if(length==7)
    for(c0=end; c0>=start; c0--)
      for(c1=start; c1<=end; c1++)
	for(c2=end; c2>=start; c2--)
	  for(c3=start; c3<=end; c3++)
	    for(c4=end; c4>=start; c4--)
	      for(c5=start; c5<=end; c5++)
		for(c6=end; c6>=start; c6--)
		  printf("%d%d%d%d%d%d%d\n",
			 c6,c5,c4,c3,c2,c1,c0);

  if(length==6)
    for(c0=end; c0>=start; c0--)
      for(c1=start; c1<=end; c1++)
	for(c2=end; c2>=start; c2--)
	  for(c3=start; c3<=end; c3++)
	    for(c4=end; c4>=start; c4--)
	      for(c5=start; c5<=end; c5++)
		printf("%d%d%d%d%d%d\n",
		       c5,c4,c3,c2,c1,c0);

  if(length==5)
    for(c0=end; c0>=start; c0--)
      for(c1=start; c1<=end; c1++)
	for(c2=end; c2>=start; c2--)
	  for(c3=start; c3<=end; c3++)
	    for(c4=end; c4>=start; c4--)
	      printf("%d%d%d%d%d\n",
		     c4,c3,c2,c1,c0);

  if(length==4)
    for(c0=end; c0>=start; c0--)
      for(c1=start; c1<=end; c1++)
	for(c2=end; c2>=start; c2--)
	  for(c3=start; c3<=end; c3++)
	    printf("%d%d%d%d\n",
		   c3,c2,c1,c0);

  if(length==3)
    for(c0=end; c0>=start; c0--)
      for(c1=start; c1<=end; c1++)
	for(c2=end; c2>=start; c2--)
	  printf("%d%d%d\n",
		 c2,c1,c0);

  if(length==2)
    for(c0=end; c0>=start; c0--)
      for(c1=start; c1<=end; c1++)
	printf("%d%d\n",
	       c1,c0);

  if(length==1)
    for(c0=end; c0>=start; c0--)
      printf("%d\n",
	     c0);

  return 0;
};
