#include <iostream>
#include <cstdlib>
#include <string.h>
#include <vector>
#include <chrono>
#include <openssl/evp.h>
#include <openssl/conf.h>

using std::vector;

int agg_sum(vector<float> nums);

int main(int argc, char *argv[]) {
	
	int n = 1000000;
	auto start = std::chrono::high_resolution_clock::now();
	vector<float> nums(n);
	
	/* Read in data from d_enc.txt, decrypt and put in nums vector */
	

	/* Compute sum */
	std::cout << agg_sum(nums) << std::endl;

	/* Compute time */
	auto stop = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
	std::cout << "n = " << n << ". Time: " << duration.count() / float(1000000) << "secs." << std::endl;
}

int agg_sum(vector<float> nums) {
	float sum;
	for (int i = 0; i < nums.size(); i++) {
		sum += nums[i];
	}
	return sum;
}
