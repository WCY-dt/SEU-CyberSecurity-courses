/**************************************************************************
 * @author Chenyang
 * @date 2021-10-28
 * @description:
 *   Perception algorithm
 * @example 
 *   input:
----------------------------------------------------------------------------
Input the degree:
3
Input the number of samples in omega1:
4
Input samples in omega1:
0 0 0
1 0 0
1 0 1
1 1 0
Input the number of samples in omega2:
4
Input samples in omega2:
0 0 1
0 1 1
0 1 0
1 1 1
Input w1:
-1 -2 -2 0
----------------------------------------------------------------------------
 *   output:
----------------------------------------------------------------------------
3 -2 -3 1
----------------------------------------------------------------------------
 * @test passed on Ubuntu 20.04.3 LTS, gcc 9.3.0
 **************************************************************************/

#include <iostream>
#include <vector>

using namespace std;

struct Vector{
    int row;
    vector<int> vec;
};

struct TrainingSample{
    Vector vec;
    int tag;
};

auto Multiplication(Vector W, Vector X){
    int ret = 0;
    for (int i = 0; i < W.row; i++){
        ret += (W.vec[i] * X.vec[i]);
    }
    return ret;
}

auto ScalarMultiplication(int c, Vector X){
    Vector ret;
    for (int i = 0; i < X.row; i++){
        ret.vec.push_back(c * X.vec[i]);
    }
    return ret;
}

auto Addition(Vector W, Vector X){
    Vector ret;
    ret.row = W.row;
    for (int i = 0; i < W.row; i++){
        ret.vec.push_back(W.vec[i] + X.vec[i]);
    }
    return ret;
}

auto Perception(vector<TrainingSample> Ts, Vector W, int c){
    bool flag = false;
    Vector w_pre = W;
    Vector w_tmp;
    int k = 0;
    w_pre.row = w_tmp.row = W.row;
    while (!flag){
        k++;
        flag = true;
        for (int i = 0; i < Ts.size(); i++){
            if (Multiplication(w_pre, Ts[i].vec) > 0){
                w_tmp = w_pre;
            }
            else{
                w_tmp = Addition(w_pre, ScalarMultiplication(c, Ts[i].vec));
                w_pre = w_tmp;
                flag = false;
            }
        }
    }
    return w_tmp;
}

int main(){

    /**
     * @description: input
     */
    vector<TrainingSample> Ts;
    Vector w1;
    int degree, numOfOmega1, numOfOmega2;

    cout << "Input the degree:\n";cin >> degree;

    cout << "Input the number of samples in omega1:\n";cin >> numOfOmega1;
    cout << "Input samples in omega1:\n";
    for (auto i = 0; i < numOfOmega1; i++){
        int x;TrainingSample tmpTrainingSample;
        tmpTrainingSample.vec.row = degree + 1;
        for (auto j = 0; j < degree; j++){cin >> x;tmpTrainingSample.vec.vec.push_back(x);}
        tmpTrainingSample.vec.vec.push_back(1);tmpTrainingSample.tag = 1;
        Ts.push_back(tmpTrainingSample);
    }

    cout << "Input the number of samples in omega2:\n";cin >> numOfOmega2;
    cout << "Input samples in omega2:\n";
    for (auto i = 0; i < numOfOmega2; i++){
        int x;TrainingSample tmpTrainingSample;
        tmpTrainingSample.vec.row = degree + 1;
        for (auto j = 0; j < degree; j++){cin >> x;tmpTrainingSample.vec.vec.push_back(-x); }
        tmpTrainingSample.vec.vec.push_back(-1);tmpTrainingSample.tag = 2;
        Ts.push_back(tmpTrainingSample);
    }

    cout << "Input w1:\n";
    int x;
    for (auto i = 0; i < degree + 1; i++){
        cin >> x;w1.vec.push_back(x);
        w1.row = degree + 1;
    }

    /**
     * @description: main function
     */
    Vector ans = Perception(Ts, w1, 1);

    /**
     * @description: output
     */
    for (auto i = 0; i < ans.row; i++){
        cout << ans.vec[i] << ' ';
    }
    cout << endl;

    return 0;
}