#include <iostream>
#include <cstdlib>
#include <string.h>
#include <chrono>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char *argv[]) {
	
	int data = open("data.txt", O_RDONLY);
	if (!data) {
		fprintf(stderr, "Unable to open file: %s\n", strerror(errno));
		return EXIT_FAILURE;
	}

	char buffer[BUFSIZ];
	auto start = std::chrono::high_resolution_clock::now();
	while (read(data, buffer, 17)) {
		// do nothing
	}
	auto stop = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
	std::cout << duration.count() << std::endl;
}
