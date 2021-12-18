#include<stdio.h> 



double test0(double x ){
	return x*x;
}
double  test1(double x){
	return x*x*x;
}

int main(){
	double (*testing[])(double) = {
		test0,
		test1,
		0
	};
	for(int i=0;testing[i];i++)
		printf("%f\n", testing[i](5));
	return 0;
}
