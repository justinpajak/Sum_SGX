#include <iostream>
#include <cstdlib>
#include <string.h>
#include <vector>
#include <chrono>
#include <unistd.h>
#include <fcntl.h>

using std::vector;

double agg_sum(vector<float> nums);

void readNums(vector<float>& nums);

int main(int argc, char *argv[]) {
	
	int n = 1000000;
	auto start = std::chrono::high_resolution_clock::now();
	vector<float> nums(n);
	
	/* Read in data from data.txt */
	readNums(nums);

	/* Compute sum */
	double sum = agg_sum(nums);
	//std::cout << std::fixed << sum << std::endl;

	/* Compute time */
	auto stop = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
	std::cout << duration.count() / float(1000000) << std::endl;
	return 0;
}

void readNums(vector<float>& nums) {
	int data = open("data.txt", O_RDONLY);
	if (!data) {
		fprintf(stderr, "Unable to open file: %s\n", strerror(errno));
		return;
	}
	char buffer[BUFSIZ];
	while(read(data, buffer, 17)) {
		nums.push_back(std::stod(buffer));
	}
}

double agg_sum(vector<float> nums) {
	double sum;
	for (int i = 0; i < nums.size(); i++) {
		sum += nums[i];
	}
	return sum;
}
