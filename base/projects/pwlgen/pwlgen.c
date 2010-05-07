
/*
 *
 * 1 -> 12 Byte PWL Generator (numeric)
 *
 * $Id$
 *
 */

#include <stdio.h>
#include <stdlib.h>

int main(void)
{
	int cb,ca,c9,c8,c7,c6,c5,c4,c3,c2,c1,c0;
	int start,end,length;

	printf("Password Length (0-16): ");
	scanf("%d[0-16]", &length);

	c6=c4=c2=c0=9; c7=c5=c3=c1=0;

	start=0; end=9;

                if(length==12)
                        for(c0=9; c0>=start; c0--)
                        for(c1=0; c1<=end; c1++)
                        for(c2=9; c2>=start; c2--)
                        for(c3=0; c3<=end; c3++)
                        for(c4=9; c4>=start; c4--)
                        for(c5=0; c5<=end; c5++)
                        for(c6=9; c6>=start; c6--)
                        for(c7=0; c7<=end; c7++)
                        for(c8=9; c8>=start; c8--)
                        for(c9=0; c9<=end; c9++)
                        for(ca=9; ca>=start; ca--)
                        for(cb=0; cb<=end; cb++)
                                printf("%d%d%d%d%d%d%d%d%d%d%d%d\n",
						cb,ca,c9,c8,c7,c6,c5,c4,c3,c2,c1,c0);

                if(length==11)
                        for(c0=9; c0>=start; c0--)
                        for(c1=0; c1<=end; c1++)
                        for(c2=9; c2>=start; c2--)
                        for(c3=0; c3<=end; c3++)
                        for(c4=9; c4>=start; c4--)
                        for(c5=0; c5<=end; c5++)
                        for(c6=9; c6>=start; c6--)
                        for(c7=0; c7<=end; c7++)
                        for(c8=9; c8>=start; c8--)
                        for(c9=0; c9<=end; c9++)
                        for(ca=9; ca>=start; ca--)
                                printf("%d%d%d%d%d%d%d%d%d%d%d\n",
						ca,c9,c8,c7,c6,c5,c4,c3,c2,c1,c0);

                if(length==10)
                        for(c0=9; c0>=start; c0--)
                        for(c1=0; c1<=end; c1++)
                        for(c2=9; c2>=start; c2--)
                        for(c3=0; c3<=end; c3++)
                        for(c4=9; c4>=start; c4--)
                        for(c5=0; c5<=end; c5++)
                        for(c6=9; c6>=start; c6--)
                        for(c7=0; c7<=end; c7++)
                        for(c8=9; c8>=start; c8--)
                        for(c9=0; c9<=end; c9++)
                                printf("%d%d%d%d%d%d%d%d%d%d\n",
						c9,c8,c7,c6,c5,c4,c3,c2,c1,c0);

		if(length==9)
			for(c0=9; c0>=start; c0--)
			for(c1=0; c1<=end; c1++)
			for(c2=9; c2>=start; c2--)
			for(c3=0; c3<=end; c3++)
			for(c4=9; c4>=start; c4--)
			for(c5=0; c5<=end; c5++)
			for(c6=9; c6>=start; c6--)
			for(c7=0; c7<=end; c7++)
			for(c8=9; c8>=start; c8--)
				printf("%d%d%d%d%d%d%d%d%d\n",
						c8,c7,c6,c5,c4,c3,c2,c1,c0);

		if(length==8)
			for(c0=9; c0>=start; c0--)
			for(c1=0; c1<=end; c1++)
			for(c2=9; c2>=start; c2--)
			for(c3=0; c3<=end; c3++)
			for(c4=9; c4>=start; c4--)
			for(c5=0; c5<=end; c5++)
			for(c6=9; c6>=start; c6--)
			for(c7=0; c7<=end; c7++)
				printf("%d%d%d%d%d%d%d%d\n",
						c7,c6,c5,c4,c3,c2,c1,c0);

		if(length==7)
                        for(c0=9; c0>=start; c0--)
                        for(c1=0; c1<=end; c1++)
                        for(c2=9; c2>=start; c2--)
                        for(c3=0; c3<=end; c3++)
                        for(c4=9; c4>=start; c4--)
                        for(c5=0; c5<=end; c5++)
                        for(c6=9; c6>=start; c6--)
                                printf("%d%d%d%d%d%d%d\n",
						c6,c5,c4,c3,c2,c1,c0);

		if(length==6)
                        for(c0=9; c0>=start; c0--)
                        for(c1=0; c1<=end; c1++)
                        for(c2=9; c2>=start; c2--)
                        for(c3=0; c3<=end; c3++)
                        for(c4=9; c4>=start; c4--)
                        for(c5=0; c5<=end; c5++)
                                printf("%d%d%d%d%d%d\n",
						c5,c4,c3,c2,c1,c0);

		if(length==5)
                        for(c0=9; c0>=start; c0--)
                        for(c1=0; c1<=end; c1++)
                        for(c2=9; c2>=start; c2--)
                        for(c3=0; c3<=end; c3++)
                        for(c4=9; c4>=start; c4--)
                                printf("%d%d%d%d%d\n",
						c4,c3,c2,c1,c0);

		if(length==4)
                        for(c0=9; c0>=start; c0--)
                        for(c1=0; c1<=end; c1++)
                        for(c2=9; c2>=start; c2--)
                        for(c3=0; c3<=end; c3++)
                                printf("%d%d%d%d\n",
						c3,c2,c1,c0);

		if(length==3)
                        for(c0=9; c0>=start; c0--)
                        for(c1=0; c1<=end; c1++)
                        for(c2=9; c2>=start; c2--)
                                printf("%d%d%d\n",
						c2,c1,c0);

		if(length==2)
                        for(c0=9; c0>=start; c0--)
                        for(c1=0; c1<=end; c1++)
                                printf("%d%d\n",
						c1,c0);

		if(length==1)
                        for(c0=9; c0>=start; c0--)
                                printf("%d\n",
						c0);

	return 0;
};
