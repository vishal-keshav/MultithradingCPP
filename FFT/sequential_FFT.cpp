//[TO-DO] Add the template instead of double
//[TO-DO] Unite the two function show_vector and show_complex based on template
//[TO-DO] Extend for general non 2^k case
#include <iostream>
#include <cstdlib>
#include <complex>
#include <cmath>
#include <vector>

using namespace std;
//Value of pi is approximated from tan inverse
double const PI = 4*atan(1);

//Filter filters out the even or odd index coeffecient
//If k==0, even indexed coefficient is returned
//If k==1, odd indexed coefficient is returned
vector<double> filter(vector<double> &coeff,int k){
	int n=coeff.size();
	int j=0;
	vector<double> ret(n/2,0);

	for(int i=0;i<n;i=i+2){
		ret[j] = coeff[i+k];
		j++;
	}
	return ret;
}

//Filter filters out the even or odd index coeffecient
//If k==0, even indexed coefficient is returned
//If k==1, odd indexed coefficient is returned
vector<complex<double> > filter_imag(vector<complex<double> > &coeff,int k){
	int n=coeff.size();
	int j=0;
	vector<complex<double> > ret(n/2);

	for(int i=0;i<n;i=i+2){
		ret[j] = coeff[i+k];
		j++;
	}
	return ret;
}
/*
//Given the coefficient, this function evaluates the 
//polynomial at a given complex point
complex<double> fun(vector<double> c,complex<double> x){
	complex<double> ret(0,0);
	complex<double> w(1,0);
	int len = c.size();
	for(int i=0;i<len;i++){
		ret = ret + w*c[i];
		w = w*x;
	}
	return ret;
}
//Evaluates the poynomial at nth roots of unity
vector<complex<double> >eval(vector<double> coeff){
	int n = coeff.size();
	complex<double> w_n(cos((2*PI)/n),sin((2*PI)/n));
	complex<double> w(1,0);
	vector<complex<double> > y(n);
	for(int i=0;i<n;i++){
		y[i] = fun(coeff,w);
		w = w*w_n;
	}
	return y;
}
*/
//Recursive FFT - serial version
vector<complex<double> > recursiveFFT(vector<double> coeff){
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
		//Even and odd coefficient are sorted out
		vector<double> coeff_even = filter(coeff,0);
		vector<double> coeff_odd = filter(coeff,1);
		//Recursively evalutaes polynomial at even and odd coefficient
		vector<complex<double> > y_even = recursiveFFT(coeff_even);
		vector<complex<double> > y_odd = recursiveFFT(coeff_odd);
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
vector<complex<double> > point_multiply(vector<complex<double> > p1,vector<complex<double> > p2){
	int n = p1.size();
	vector<complex<double> > ret(n);
	if(n!=p2.size()){
		cout << "Size does not match" << endl;
	}
	else{
		for(int i=0;i<n;i++){
			ret[i] = p1[i]*p2[i];
		}
	}
	return ret;
}

//Converts from n points to n-1 degree polynomial
vector<complex<double> > recursiveFFTinv(vector<complex<double> > points){
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
		//Even and odd coefficient are sorted out
		vector<complex<double> > points_even = filter_imag(points,0);
		vector<complex<double> > points_odd = filter_imag(points,1);
		//Recursively evalutaes polynomial at even and odd coefficient
		vector<complex<double> > y_even = recursiveFFTinv(points_even);
		vector<complex<double> > y_odd = recursiveFFTinv(points_odd);
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
vector<double> recursiveFFTinv_wrapper(vector<complex<double> > points){
	vector<complex<double> > coeff = recursiveFFTinv(points);
	int n = coeff.size()-1;
	vector<double> ret(n,0);
	
	for(int i=0;i<n;i++){
		ret[i] = real(coeff[i])/(n+1);
	}
	return ret;
}

//This is simple convolution function of O(n^2)
vector<double> prod(vector<double> a,vector<double> b){
	vector<double> ret(a.size()+b.size()-1,0);
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
vector<double> prod_fourier(vector<double> c1,vector<double> c2){
	int n = c1.size();
	c1.resize(2*n);
	c2.resize(2*n);
	vector<complex<double> > vec1 = recursiveFFT(c1);
	vector<complex<double> > vec2 = recursiveFFT(c2);
	vector<complex<double> > multi = point_multiply(vec1,vec2);
	vector<double> ret = recursiveFFTinv_wrapper(multi);
	return ret;
}

/*
void show_vector(vector<double> v){
	int len = v.size();
	for(int i=0;i<len;i++){
		cout << v[i] << " ";
	}
	cout << endl;
}

void show_complex(vector<complex<double> > v){
	int len = v.size();
	for(int i=0;i<len;i++){
		cout << v[i] << " ";
	}
	cout << endl;
}
*/
////////////////////////////////////////////////////////////////////////////////

int main(){
	int n=pow(2,13);

	vector<double> coeff1,coeff2;
	for(int i=0;i<n;i++){
		coeff1.push_back((rand()%10)+1);
		coeff2.push_back((rand()%10)+1);
	}

	clock_t startTime;
	startTime = clock();
	vector<double> coeff_ans1 = prod(coeff1,coeff2);
	cout << double( clock() - startTime ) / (double)CLOCKS_PER_SEC<< " seconds." << endl;
	//show_vector(coeff_ans1);
	startTime = clock();
	vector<double> coeff_ans2 = prod_fourier(coeff1,coeff2);
	cout << double( clock() - startTime ) / (double)CLOCKS_PER_SEC<< " seconds." << endl;
	//show_vector(coeff_ans2);

	return 0;
}