#include "matmul.hpp"


/* Layer 1 matrix multiplication */
void hwmm_layer1(float input[n_inputs], const float weights[n_inputs][n_layer1], mytype output[1][n_layer1]) {
    col: for (int j = 0; j < n_layer1; ++j) {
#pragma HLS unroll factor=2
    	mytype sum = 0;

      prod: for (int k = 0; k < n_inputs; ++k){
#pragma HLS UNROLL factor=2

        sum += FLOAT_TO_FIXED(input[k] * weights[k][j],N);
      }

      output[0][j] = sum;
    }

  return;
}



/* Layer 2 matrix multiplication */
void hwmm_layer2(mytype input[1][n_layer1], const float weights[n_layer1][n_layer2], mytype output[1][n_layer2]) {

    col: for (int j = 0; j < n_layer2; ++j) {
//#pragma HLS UNROLL factor = 2
    	mytype sum = 0;

      prod: for (int k = 0; k < n_layer1; ++k){
//#pragma HLS UNROLL factor = 2
        sum += MULTIPLY_FIXED(input[0][k],FLOAT_TO_FIXED(weights[k][j],N),N);
      }
      std::cout << FIXED_TO_FLOAT(sum,N) << std::endl;
      output[0][j] = sum;
    }


  return;
}






/* ReLU layer 1 activation function */
void hw_act_layer1(mytype input[1][n_layer1], mytype output[1][n_layer1]){
	loop1: for (int i = 0; i < n_layer1; i++){
#pragma HLS UNROLL
		if (FIXED_TO_FLOAT(input[0][i],N) < 0.0)
			output[0][i] = 0;
		else
			output[0][i] = input[0][i];
	}

	return;
}



/* Softmax layer 2 activation function */
void hw_act_layer2(mytype input[1][n_layer2], int &pred){
	int max_idx = -1;
	float max_val = -999.9;
	loop1: for (int i = 0; i < n_layer2; i++){
#pragma HLS UNROLL
		if (FIXED_TO_FLOAT(input[0][i],N) > max_val){
			max_idx = i;
			max_val = FIXED_TO_FLOAT(input[0][i],N);
		}
	}
	pred = max_idx;
	return;
}



/* Connect NN Layers */
int nn_inference(float input_img[n_inputs]){
//#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=input_img
//#pragma HLS INTERFACE ap_memory storage_type=ram_1p port=input_img
//#pragma HLS INTERFACE s_axilite port=return bundle=AXI_CPU


	mytype temp_output[1][n_layer1] = {1};
	mytype temp_output2[1][n_layer2] = {1};
	int prediction = -1;

	hwmm_layer1(input_img, weights::layer1_weights, temp_output);
	hw_act_layer1(temp_output, temp_output);
	hwmm_layer2(temp_output, weights::layer2_weights, temp_output2);
	hw_act_layer2(temp_output2, prediction);

	return prediction;

}

