#include<iostream>
#include <random>
#include<iomanip>

using namespace std;

const float PI = 3.1415926535f;

//随机数生成器
class RandomGenerator {
	private:
		std::uniform_real_distribution<double> distribution;
		std::mt19937 generator;

	public:
		RandomGenerator() {
			distribution = std::uniform_real_distribution<double>(0.0, 1.0);
		}

		float random_float(){
            return (float)distribution(generator);
        }

		float random_float(float x, float y){
            return x + (y - x) * random_float();
        }
};
static RandomGenerator randomGenerator;

//正态分布生成器
class NormalDistributionGenerator {
	private:
		std::default_random_engine generator;
        std::normal_distribution<double> normalDistribution;
	public:
		NormalDistributionGenerator(float miu = 0.f, float sigma = 1.f) {
			normalDistribution = std::normal_distribution<double>(miu, sigma);
		}

        void ResetPara(float miu = 0.f, float sigma = 1.f){
            normalDistribution = std::normal_distribution<double>(miu, sigma);
        }

		float random_float(){
            return (float)normalDistribution(generator);
        }

		float random_float(float x, float y){
            float u = random_float();
            while(u<x || u>y) u = random_float();
            return u;
        }
};

//待求解函数
float f(float x){
    return x*x*x;
    //return exp(-5*pow(x-3,6));
}

//均匀分布概率密度函数
float pdf1(float x, float lowBound, float highBound){
    return 1.f/(highBound - lowBound);
}

//正态分布概率密度函数
float pdf2(float x, float miu, float sigma){
    return (1.f/(sigma*pow(2*PI,0.5f)))*exp(-(0.5f)*pow(x-miu,2)/(sigma*sigma));
}

//正态分布的三个百分比
float dises[3] = {0.6827f, 0.9545f, 0.9973f};

//蒙特卡洛积分
void MonteCarlo(int n, float lowBound, float highBound){
    float s1 = 0.f;
    float s2_1 = 0.f,s2_2 = 0.f,s2_3 = 0.f;

    float miu =(lowBound+highBound)/2.f;
    const float sigma[] = {
        (highBound - lowBound)/2.f,
        (highBound - lowBound)/4.f,
        (highBound - lowBound)/6.f
    };
    NormalDistributionGenerator g[3];
    for(int i=0;i<3;i++){
        g[i] = NormalDistributionGenerator(miu, sigma[i]);
    }

    for(int i = 0; i < n; i++){
        float u1 = randomGenerator.random_float(lowBound, highBound);
        float u2_1 = g[0].random_float(lowBound, highBound);
        float u2_2 = g[1].random_float(lowBound, highBound);
        float u2_3 = g[2].random_float(lowBound, highBound);
        //while(u2>highBound || u2<lowBound) u2 = normalGenerator(generator);
        float p1 = pdf1(u1, lowBound, highBound);
        float p2_1 = pdf2(u2_1, miu, sigma[0]) / dises[0];
        float p2_2 = pdf2(u2_2, miu, sigma[1]) / dises[1];
        float p2_3 = pdf2(u2_3, miu, sigma[2]) / dises[2];
        s1 += f(u1) / (p1*n);
        s2_1 += f(u2_1) / (p2_1*n);
        s2_2 += f(u2_2) / (p2_2*n);
        s2_3 += f(u2_3) / (p2_3*n);
    }
    std::cout<<"n:"<<setw(8)<<n<<":   ";
    std::cout<<"s1:"<<setiosflags(ios::fixed)<<setprecision(6)<<s1<<";   ";
    std::cout<<"s2_1:"<<setiosflags(ios::fixed)<<setprecision(6)<<s2_1<<";   ";
    std::cout<<"s2_2:"<<setiosflags(ios::fixed)<<setprecision(6)<<s2_2<<";   ";
    std::cout<<"s2_3:"<<setiosflags(ios::fixed)<<setprecision(6)<<s2_3<<"\n";
}

int main(){
    for(int i=0;i<=16;i+=2){
        int n = pow(2, 5 + i);
        MonteCarlo(n, 1.f, 3.f);
        std::cout<<endl;
    }
    return 0;
}
