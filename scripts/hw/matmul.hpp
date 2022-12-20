#include "ap_int.h"
#include <ap_fixed.h>


#define n_inputs 18*32
#define n_layer1 80
#define n_layer2 5


typedef ap_fixed<20,5,AP_RND> myfloat;
//typedef double myfloat;


void hwmm_layer1(myfloat input[n_inputs], const myfloat weights[n_inputs][n_layer1], myfloat output[1][n_layer1]);
void hw_act_layer1(myfloat input[1][n_layer1], myfloat output[1][n_layer1]);
void hwmm_layer2(myfloat input[1][n_layer1], const myfloat weights[n_layer1][n_layer2], myfloat output[1][n_layer2]);
void hw_act_layer2(myfloat input[1][n_layer2], int &pred);
int nn_inference(myfloat input_img[n_inputs]);


namespace weights{
