#include <iostream>
#include <cstdlib>
#include <complex>
#include <cmath>
#include <vector>
#include <omp.h>

using namespace std;
//Value of pi is approximated from tan inverse
double const PI = 4*atan(1);

//Prints elements of a  generic vector
template <class T>
void show_vector(vector<T> v){
	int len = v.size();
	for(int i=0;i<len;i++){
		cout << v[i] << " ";
	}
	cout << endl;
}

//Generate next number which is power of 2
int next_powerof2(int v){
	int r;
	if (v > 1){
  		float f = (float)v;
  		unsigned int const t = 1U << ((*(unsigned int *)&f >> 23) - 0x7f);
  		r = t << (t < v);
	}
	else {
  		r = 1;
	}
}

//Filter filters out the even or odd index coeffecient
//If k==0, even indexed coefficient is returned
//If k==1, odd indexed coefficient is returned
template <class T>
vector<T> filter(vector<T> &coeff,int k){
	int n=coeff.size();
	int j=0;
	vector<T> ret(n/2,0);
	#pragma omp parallel for
	for(int i=0;i<n;i=i+2){
		ret[j] = coeff[i+k];
		j++;
	}
	return ret;
}

//Recursive FFT - serial version
template <class T>
vector<complex<double> > recursiveFFT(vector<T> &coeff){
	int n=coeff.size();
	//Base case
	if(n==1){
		vector<complex<double> > v(1);
		v[0] = complex<double>(coeff[0],0);
		return v;
	}
	//Main case
	else{
		complex<double> w_n(cos((2*PI)/n),sin((2*PI)/n));
		complex<double> w(1,0);
		vector<complex<double> > y_even;
		vector<complex<double> > y_odd;
		//Even and odd coefficient are sorted out
		#pragma omp parallel sections
		{
			{vector<double> coeff_even = filter(coeff,0);
			y_even = recursiveFFT(coeff_even);}
			#pragma omp section
			{
				vector<double> coeff_odd = filter(coeff,1);
				y_odd = recursiveFFT(coeff_odd);
			}
		}
		vector<complex<double> > y(n);
		for(int k=0;k<n/2;k++){
			y[k] = y_even[k] + w*y_odd[k];
			y[k + n/2] = y_even[k] - w*y_odd[k];
			w = w*w_n;
		}
		return y;
	}
}

//Multiplies two vectors of complex numbers component wise
template <class T>
vector<T> point_multiply(vector<T> &p1,vector<T> &p2){
	int n = p1.size();
	vector<T> ret(n);
	if(n!=p2.size()){
		cout << "Size does not match" << endl;
	}
	else{
		#pragma omp parallel for
		for(int i=0;i<n;i++){
			ret[i] = p1[i]*p2[i];
		}
	}
	return ret;
}

//Converts from n points to n-1 degree polynomial
vector<complex<double> > recursiveFFTinv(vector<complex<double> > &points){
	int n = points.size();
	//Base case
	if(n==1){
		vector<complex<double> > v(1);
		v[0] = points[0];
		return v;
	}
	//Main case
	else{
		complex<double> w_n(cos((-2*PI)/n),sin((-2*PI)/n));
		complex<double> w(1,0);
		vector<complex<double> > y_even;
		vector<complex<double> > y_odd;
		//Even and odd coefficient are sorted out
		#pragma omp parallel sections
		{
			{vector<complex<double> > points_even = filter(points,0);
			y_even = recursiveFFTinv(points_even);}
			#pragma omp section
			{
				vector<complex<double> > points_odd = filter(points,1);
				//Recursively evalutaes polynomial at even and odd coefficient
				y_odd = recursiveFFTinv(points_odd);
			}
		}
		vector<complex<double> > y(n);		
		for(int k=0;k<n/2;k++){
			y[k] = y_even[k] + w*y_odd[k];
			y[k + n/2] = y_even[k] - w*y_odd[k];
			w = w*w_n;
		}
		return y;
	}
}

//Its a wrapper function for inverse DFT that sepeates out imagenary part

vector<double> recursiveFFTinv_wrapper(vector<complex<double> > &points){
	vector<complex<double> > coeff = recursiveFFTinv(points);
	int n = coeff.size()-1;
	vector<double> ret(n,0);
	#pragma omp parallel for
	for(int i=0;i<n;i++){
		ret[i] = real(coeff[i])/(n+1);
	}
	return ret;
}

//This is simple convolution function of O(n^2)
template <class T>
vector<T> prod(vector<T> a,vector<T> b){
	vector<T> ret(a.size()+b.size()-1,0);
	a.resize(ret.size());
	b.resize(ret.size());
	for(int i=0;i<ret.size();i++){
		ret[i]=0;
		for(int j=0;j<=i;j++){
			ret[i] = ret[i] + a[j]*b[i-j];
		}
	}
	return ret;
}

//Wrapper for polynomial multiplication in O(nlog(n))
template <class T>
vector<double> prod_fourier(vector<T> c1,vector<T> c2){
	int n = c1.size();
	c1.resize(2*n);
	c2.resize(2*n);
	vector<complex<double> > vec1;
	vector<complex<double> > vec2;
	#pragma omp parallel sections
	{
		{vec1 = recursiveFFT(c1);}
		#pragma omp section
		{
			vec2 = recursiveFFT(c2);
		}
	}
	vector<complex<double> > multi = point_multiply(vec1,vec2);
	vector<T> ret = recursiveFFTinv_wrapper(multi);
	return ret;
}

////////////////////////////////////////////////////////////////////////////////

int main(){
	//For non 2^k elements, use the function next_powerof2 defined above
	int n=pow(2,13);

	//Data creation, data type is double. It can be anything like int
	vector<double> coeff1,coeff2;
	for(int i=0;i<n;i++){
		coeff1.push_back((rand()%10)+1);
		coeff2.push_back((rand()%10)+1);
	}

	clock_t startTime;
	//O(n^2)
	startTime = clock();
	vector<double> coeff_ans1 = prod(coeff1,coeff2);
	cout << double( clock() - startTime ) / (double)CLOCKS_PER_SEC<< " seconds." << endl;
	//show_vector(coeff_ans1);

	//O(nlog(n))
	startTime = clock();
	
	vector<double> coeff_ans2 = prod_fourier(coeff1,coeff2);
	cout << double( clock() - startTime ) / (double)CLOCKS_PER_SEC<< " seconds." << endl;
	//show_vector(coeff_ans2);

	return 0;
}