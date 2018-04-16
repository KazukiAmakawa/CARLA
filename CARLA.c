//###########################################################
//
//		CARLA
//		Copyright(c) KazukiAmakawa, all right reserved.
//		CARLA.c
//
//###########################################################

#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <memory.h>


const double pi = 3.141592653589793;
const double e = 2.718281828459045;
const int InteSize = 500;
const int SizeOfR = 500;
const double omega = 1000;

double Cost(int var, double Parameter[var]);
int sb;								//For test
double Histogram[256];

/*===================DO NOT CHANGE ANYTHING BELOW===================*/


double Random(){
/*	
	//Function Instruction:
	This function will return a random number with seed

	//Parameter Instruction:
	int seed = Random number getting seed;

	return A random number;
*/

	//Definition and Initialization
	double num;

	//Main Loop, get random number
	int i;
	for(i = 0; i < 10; i ++)
		num = (double)rand()/RAND_MAX;
	
	//Output and return
	return num;
}


/*
Here is a struct using for quick sort
*/
typedef struct _Range {
	int start, end;
} Range;
Range new_Range(int s, int e) {
	Range r;
	r.start = s;
	r.end = e;
	return r;
}
void swap(double *x, double *y) {
	double t = *x;
	*x = *y;
	*y = t;
}


double quick_sort(const int len, double arr[len]) {
/*	
	//Function Instruction:
	This function will satisfied the quick sort algorithm.

	//Parameter Instruction:
	const int len = Length of the sort array
	double arr[len] = Array you want to sort

	return medium value of all array;
*/
	Range r[len];
	int p = 0;
	r[p++] = new_Range(0, len - 1);
	while (p) {
		Range range = r[--p];
		if (range.start >= range.end)
			continue;
		double mid = arr[range.end];
		int left = range.start, right = range.end - 1;
		while (left < right) {
			while (arr[left] < mid && left < right)
				left ++;
			while (arr[right] >= mid && left < right)
				right --;
			swap(&arr[left], &arr[right]);
		}
		if (arr[left] >= arr[range.end])
			swap(&arr[left], &arr[range.end]);
		else
			left++;
		r[p++] = new_Range(range.start, left - 1);
		r[p++] = new_Range(left + 1, range.end);
	}

	return arr[(int)(len/2)];
}


void Algorithm(const int var, const int ttl, const double gw, const double gh, const char command, double Interval[var][2]){
/*	
	//Function Instruction:
	This function is main function of CARLA Method.

	//Parameter Instruction:
	const int var = Total parameter will learning;
	const int ttl = Total iterator loop during the learning ;
	const double gw = Parameter gw of CARLA method;
	const double gh = Parameter gh of CARLA method;
	const char command = 
		"p" means this algorithm is using for presentation, it will calculate all PDF value
		"t" means this algorithm is using for test, it will print some test parameter
		"w" means this algorithm is using for working, it will output only main result
	Interval[var][2] = [[X1Min, X1Max], ... ,[XnMin, XnMax]] where n = var;

	return Learning result array;
*/

	//Saving Definition
	double x[var][ttl];					//To save all decision point
	double J[ttl];						//To save all cost value
	double Jmed;						//To save medium value of all J
	double Jmin;						//To save minumum value of all J
	double alpha[var][ttl + 1];			//To save all PDF parameter
	double beta[ttl + 1];				//To save all reinforcement parameter
	double lambda[var];					//To save all lambda parameter
	double sigma[var];					//To save all sigma parameter

	memset(x, 0, sizeof(x));
	memset(J, 0, sizeof(J));
	memset(alpha, 0, sizeof(alpha));
	memset(beta, 0, sizeof(beta));
	memset(lambda, 0, sizeof(lambda));
	memset(sigma, 0, sizeof(sigma));

	int positive;						//For test

	srand(time(NULL));					//For random number getting
	double z;							//A random number which is the PDF integral value
	double tem;							//A temple tank for saving calculation value

	//Pretreatment, Calculate lambda and sigma
	int i;
	for(i = 0; i < var; i ++){
		sigma[i] = gw * (Interval[i][1] - Interval[i][0]); 
		lambda[i] = gh / (Interval[i][1] - Interval[i][0]);
		
	}


	//Main Loop
	int kase;
	for(kase = 0; kase < ttl; kase ++){
		int par;
		for(par = 0; par < var; par ++){
	/*
		==========Get the random number z, which is the PDF integral value==========
	*/
			z = Random();

	/*
		========================Newton's Method, get x========================
	*/	
			//Definition and Initialization
			double delta = (Interval[par][0] + Interval[par][1]) / 2;
			double Remdelta = 0;


			/*Newton's Method*/
			/*	STILL HAVE BUG!!
			//Main Loop
			int loop;
			for(loop = 0; loop < 1000; loop ++){
				//Integral function(Original function) integral
				double Fx = (delta - Interval[par][0]) / (Interval[par][1] - Interval[par][0]);
				int k;
				for(k = 1; k <= kase; k ++){
					tem = erf( (delta - x[par][k-1]) / (sqrt(2) * sigma[par]) ) - erf( (Interval[par][0] - x[par][k-1]) / (sqrt(2) * sigma[par]) );
					tem = beta[par][k] * lambda[par] * sigma[par] * sqrt(2 * pi) / 2 * tem;
					Fx = alpha[par][k] * (Fx + tem);
				}
				//Normal function(Partial function) integral
				double dFx = 1 / (Interval[par][1] - Interval[par][0]);
				for(k = 1; k <= kase; k ++){
					tem = exp(- pow((delta - x[par][k-1]), 2) / (2 * sigma[par] * sigma[par]) );
					tem = beta[par][k] * lambda[par] * tem;
					dFx = alpha[par][k] * (dFx + tem);
				}

				//Newton's iterator
				Remdelta = delta;
				delta = delta - (Fx - z) / dFx;

				//Condition judgement
				if ((delta - Remdelta < 0.0001) && (delta - Remdelta > -0.0001))
					break;
			}

			if (delta > Interval[par][1])				x[par][kase] = Interval[par][1];
			else if(delta < Interval[par][0])			x[par][kase] = Interval[par][0];
			else										x[par][kase] = delta;
			*/
			/*Newton's Method END*/


			/*Bisection Method*/
			double Left = Interval[par][0];
			double Right = Interval[par][1];
			double MedVal;
			int loop, k;
			double FLeft, FRight, FMed;
			
			
			//printf("func\t");
			FLeft = (Left - Interval[par][0]) / (Interval[par][1] - Interval[par][0]);
			for(k = 1; k <= kase; k ++){
				tem = erf( (Left - x[par][k-1]) / (sqrt(2) * sigma[par]) ) - erf( (Interval[par][0] - x[par][k-1]) / (sqrt(2) * sigma[par]) );
				tem = beta[k] * lambda[par] * sigma[par] * sqrt(2 * pi) / 2 * tem;
				FLeft = alpha[par][k] * (FLeft + tem);
			}
			FLeft -= z;

			FRight = (Right - Interval[par][0]) / (Interval[par][1] - Interval[par][0]);
			for(k = 1; k <= kase; k ++){
				tem = erf( (Right - x[par][k-1]) / (sqrt(2) * sigma[par]) ) - erf( (Interval[par][0] - x[par][k-1]) / (sqrt(2) * sigma[par]) );
				tem = beta[k] * lambda[par] * sigma[par] * sqrt(2 * pi) / 2 * tem;
				FRight = alpha[par][k] * (FRight + tem);
			}
			FRight -= z;
			
			positive = 0;
			for(loop = 0; loop < 1000; loop ++){
				positive += 1;
				//printf("%d\n", sb);
				MedVal = (Left + Right) / 2;
				double FMed = (MedVal - Interval[par][0]) / (Interval[par][1] - Interval[par][0]);
				for(k = 1; k <= kase; k ++){
					tem = erf( (MedVal - x[par][k-1]) / (sqrt(2) * sigma[par]) ) - erf( (Interval[par][0] - x[par][k-1]) / (sqrt(2) * sigma[par]) );
					tem = beta[k] * lambda[par] * sigma[par] * sqrt(2 * pi) / 2 * tem;
					FMed = alpha[par][k] * (FMed + tem);
				}
				FMed -= z;

				if((Remdelta - MedVal < 0.00000001) && (Remdelta - MedVal > -0.00000001))
					break;

				Remdelta = MedVal;
				if (FMed * FLeft < 0){
					Right = MedVal;
					FRight = FMed;
				}
				else{
					Left = MedVal;
					FLeft = FMed;
				}
			}

			x[par][kase] = MedVal;
			/*Bisection Method END*/
		}
	/*
		=========================Calculate the cost J=========================
	*/

		//Get the parameter group at present.
		double Parameter[var];
		int loop;
		for(loop = 0; loop < var; loop ++)
			Parameter[loop] = x[loop][kase];


		//Calculate the cost
		J[kase] = Cost(var, Parameter);

	/*
		====================Reflesh the medium and minimum cost====================
	*/
		//Calculation of Jmed
		//Definition and Initialization
		int TemSize;
		if (kase <= SizeOfR)							TemSize = kase + 1;
		else											TemSize = 501;

		double TemJ[TemSize];
		memcpy(TemJ, J + kase - TemSize + 1, sizeof(TemJ));
		//Here I used pointer to make the copy faster
		
		Jmed = quick_sort(TemSize, TemJ);

		//Calculation of Jmin
		if (kase == 0)		
			Jmin = J[kase];
		else				
			Jmin = (Jmin < J[kase])? Jmin : J[kase];


	/*
		====================Calculate the reinforcement value beta====================
	*/
		if ((Jmed - Jmin) < 0.000001 && (Jmed - Jmin) > -0.000001)
			beta[kase + 1] = 0;
		else{
			tem = (Jmed - J[kase]) / (Jmed - Jmin);
			beta[kase + 1] = (tem > 0)? tem : 0;
		}


		/*
			==========Calculate the PDF parameter alpha and get the PDF of next koop==========
		*/
		for(par = 0; par < var; par ++){
			double tem1 = erf( (Interval[par][1] - x[par][kase]) / (sqrt(2) * sigma[par]) );
			double tem2 = erf( (Interval[par][0] - x[par][kase]) / (sqrt(2) * sigma[par]) );
			double tem3 = tem1 - tem2;
			tem = beta[kase + 1] * lambda[par] * sigma[par] * sqrt(2 * pi) / 2;
			tem = tem * tem3;
			if (tem != 0)
				tem -= 0.001;
			alpha[par][kase + 1] = 1 / (1 + tem);
			//printf("PDF\t%.16f\t%.16f\t%.16f\t%.16f\t%.16f\t\n", tem1, tem2, tem3, tem, alpha[par][kase + 1]);
		}


		/*
			===============Tem output and confident if the algorithm is right our not===============
		*/
		if (command == 't' || command == 'p')
			for(par = 0; par < var; par ++)
				printf("sb:\t%d\t%0.16f\t%0.16f\t%0.16f\t%0.16f\t%0.16f\t%0.16f\t%0.16f\t\n", par, z, x[par][kase], J[kase], Jmed, Jmin, alpha[par][kase + 1], beta[kase + 1]);

		if (command == 'p'){
			double Output = 0;
			double LenIntervar = (double)1 / InteSize * (Interval[par][1] - Interval[par][0]);

			int ima;
			for(ima = 0; ima <= InteSize; ima ++){
				double delta = (double)ima / InteSize * (Interval[par][1] - Interval[par][0]) + Interval[par][0];				
				double total = (double)1 / (Interval[par][1] - Interval[par][0]);
				int k;
				for(k = 0; k <= kase; k ++){
					tem = beta[k + 1] * lambda[par] * exp( - pow((delta - x[par][k]), 2) / (2 * sigma[par] * sigma[par])  );
					total = alpha[par][k + 1] * (total + tem);
				}
				Output = (double)total;
				printf("%0.16f\t", Output);
				Output = 0;
			}
			printf("\n");

		}
	}

	//Judgement, calculate the exception of PDF, and make a decision
	//freopen("After", "w", stdout);
	int par;
	for(par = 0; par < var; par ++){
		double Output = 0;
		double LenIntervar = (double)1 / InteSize * (Interval[par][1] - Interval[par][0]);
		int kase;
		/*
		for(kase = 0; kase <= InteSize; kase ++){
			double delta = (double)kase / InteSize * (Interval[par][1] - Interval[par][0]) + Interval[par][0];				
			double total = (double)1 / (Interval[par][1] - Interval[par][0]);
			int k;
			for(k = 1; k <= ttl; k ++){
				tem = beta[k] * lambda[par] * exp( - pow((delta - x[par][k-1]), 2) / (2 * sigma[par] * sigma[par])  );
				total = alpha[par][k] * (total + tem);
			}
			Output += (double)total * LenIntervar * delta;
			
		}
		*/
		double maxx = 0;
		for(kase = 0; kase <= InteSize; kase ++){
			double delta = (double)kase / InteSize * (Interval[par][1] - Interval[par][0]) + Interval[par][0];				
			double total = (double)1 / (Interval[par][1] - Interval[par][0]);
			int k;
			for(k = 1; k <= ttl; k ++){
				tem = beta[k] * lambda[par] * exp( - pow((delta - x[par][k-1]), 2) / (2 * sigma[par] * sigma[par])  );
				total = alpha[par][k] * (total + tem);
			}
			//printf("%.16f\n", total);
			//scanf("%d", &sb);
			if (maxx < total){
				Output = kase;
				maxx = total;
			}
		}
		Output = Output * (Interval[par][1] - Interval[par][0]) / InteSize + Interval[par][0];
		printf("o%0.16f\n", Output);

	}
	return ;
}


/*===================DO NOT CHANGE ANYTHING ABOVE===================*/


int main(int argc, char const *argv[]){
/*	
	//Function Instruction:
	Main function, where is the begining of all program

	//Parameter Instruction:
	void;

	return 0;
*/
	//Interval definition
	freopen("Input.out", "r", stdin);
	freopen("Output.out", "w", stdout);

	int var;
	for(int var = 0; var < 256; var ++){
		scanf("%lf", &Histogram[var]);
		getchar();
	}

	int ttl, loop;
	double gw, gh;
	char mode;
	scanf("%d%d%lf%lf%s", &ttl, &loop, &gw, &gh, &mode);
	getchar();
	
	double Interval[ttl][2];
	for(var = 0; var < ttl; var ++){
		scanf("%lf%lf", &Interval[var][0], &Interval[var][1]);
		getchar();
	}
	
	Algorithm(ttl, loop, gw, gh, mode, Interval);
	
	return 0;
}


double Cost(int var, double Parameter[var]){
/*	
	//Function Instruction:
	This function will return cost in different situation, which you can change it.

	//Parameter Instruction:
	double cost valut;

	return 0;
*/	
	double Output[256];
	memset(Output, 0, sizeof(Output));
	int par, loc;
	double tem, Total = 0, Prob = 0;
	
	//SOME ERROR IN THIS PART OF CODE
	for(par = 0; par < var / 3; par ++){
		double Pr, Sigma, Mu;
		Pr = Parameter[3 * par];
		Sigma = Parameter[3 * par + 1];
		Mu = Parameter[3 * par + 2];
		for(loc = 0; loc < 256; loc ++){
			//0: Prob, 1: sigma, 2: mu
			Prob += Pr;
			tem = exp(- pow(loc - Mu, 2) / (2 * Sigma * Sigma));
			Output[loc] += (tem * Pr) / (Sigma * sqrt(2 * pi));
		}
	}
	
	//scanf("%d", &sb);
	for(loc = 0; loc < 256; loc ++)
		Total += pow((Output[loc] - Histogram[loc]), 2);
	Total /= 256;
	/*
	if (Prob <= 1)					Total = Total + omega * (1 - Prob);
	else							Total = Total + omega * (Prob - 1);
	*/
	return Total;
}






