#define E 2.71828182845904523536028747135266250;
#define FRAC_2_SQRT_PI 1.12837916709551257389615890312154517;

double erf(double x){
    if ((x <= 2.0) && (x >= 0.0)){
        double out = 1.0;
        double prev = 1.0;
        for (int i = 1; i <= 20; i++){
            prev *= (2.0*x*x)/(2*i+1);
            out += prev;
        }
        return out*x*pow(E, -x*x)*FRAC_2_SQRT_PI;
    }
    else{
        double prev = 8.0/x;
        for(int i = 0; i < 15; i++){
            prev = (15-i)/(x+prev);
        }
        return 1.0-pow(E, -x*x)/(2*prev)*FRAC_2_SQRT_PI;
    }
}