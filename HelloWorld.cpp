#define CL_USE_DEPRECATED_OPENCL_1_2_APIS
#include <CL/cl.hpp>
#include <fstream>
#include <iostream>
#include <stdlib.h>


#define STB_IMAGE_IMPLEMENTATION
#include "stb_image/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image/stb_image_write.h"



int main() {

	std::vector<cl::Platform> platforms;
	cl::Platform::get(&platforms);

	auto platform = platforms.front();
	std::vector<cl::Device>devices;
	platform.getDevices(CL_DEVICE_TYPE_GPU, &devices);

	auto device = devices.front();

	std::ifstream helloworldFile("HelloWorld.cl");
	std::string src(std::istreambuf_iterator<char>(helloworldFile), (std::istreambuf_iterator<char>()));

	cl::Program::Sources sources(1, std::make_pair(src.c_str(), src.length() + 1));

	cl::Context context(device);
	cl::Program program(context, sources);

	program.build("-cl-std=CL1.2");


	//load background
	int widthB, heightB, channelsB;
	unsigned char *background = stbi_load("t.jpg", &widthB, &heightB, &channelsB, 0);
	if (background == NULL) {
		printf("Error in loading the image\n");
		exit(1);
	}
	printf("Loaded image with a width of %dpx, a height of %dpx and %d channels\n", widthB, heightB, channelsB);


	// Convert the input image to gray
	size_t img_size = widthB * heightB * channelsB;
	printf("Image size =  %d\n", img_size);
	int gray_channels = channelsB == 4 ? 2 : 1;
	size_t gray_img_size = widthB * heightB * gray_channels;
	unsigned char *gray_img = new unsigned char[gray_img_size];
	if (gray_img == NULL) {
		printf("Unable to allocate memory for the gray image.\n");
		exit(1);
	}
	printf("Loaded image with a width of %dpx, a height of %dpx, %dpx channels and %d size\n", widthB, heightB, channelsB, gray_img_size);


	/*for (unsigned char *p = background, *pg = gray_img; p != background + img_size; p += channelsB, pg += gray_channels)
	{
		*pg = (uint8_t)((*p + *(p + 1) + *(p + 2)) / 3.0);
		if (channelsB == 4) {
			*(pg + 1) = *(p + 3);
		}
	}*/

	/*char* str = new char[gray_img_size];
	memset(str, *gray_img, gray_img_size);
	puts(str);*/

	cl_int err = 0;
	cl::Buffer entrada(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(char)*img_size, background, &err);
	cl::Buffer saida(context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(char)*gray_img_size, gray_img, &err);
	/*cl::Buffer canal1(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(int), &channelsB, &err);
	cl::Buffer canal2(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(int), &gray_channels, &err);*/
	cl::Kernel kernel(program, "ProcessArray");
	err = kernel.setArg(0, entrada);
	err = kernel.setArg(1, saida);
	/*err = kernel.setArg(2, canal1);
	err = kernel.setArg(3, canal2);*/

	cl::CommandQueue queue(context, device);
	//queue.enqueueTask(kernel);
	//queue.enqueueWriteBuffer(entrada, CL_TRUE, 0, sizeof(char)*img_size, background);
	/*queue.enqueueReadBuffer(memBuf, GL_TRUE, 0, sizeof(buf), buf);*/
	err = queue.enqueueNDRangeKernel(kernel, cl::NullRange, cl::NDRange(img_size));
	err = queue.enqueueWriteBuffer(saida, CL_TRUE, 0, sizeof(char)*gray_img_size, gray_img);

	cl::finish();

	//escrever background
	stbi_write_jpg("gray4.jpg", widthB, heightB, gray_channels, gray_img, 100);
	stbi_image_free(background);
	stbi_image_free(gray_img);

	//std::cout << buf;
	/*std::cin.get();*/
	return 0;
}


//load 2 images
//int width, height, channels;
//int width2, height2, channels2;
//unsigned char *img = stbi_load("t.jpg", &width, &height, &channels, 0);
//if (img == NULL) {
//	printf("Error in loading the image\n");
//	exit(1);
//}
//printf("Loaded image with a width of %dpx, a height of %dpx and %d channels\n", width, height, channels);
//
//unsigned char *img2 = stbi_load("s.png", &width2, &height2, &channels2, 0);
//if (img == NULL) {
//	printf("Error in loading the image\n");
//	exit(1);
//}
//printf("Loaded image with a width of %dpx, a height of %dpx and %d channels\n", width2, height2, channels2);

//write 2 images
//stbi_write_jpg("t3.jpg", width, height, channels, img, 100);
//stbi_write_png("s2.png", width2, height2, channels2, img2, width2 * channels2);
//stbi_image_free(img);
//stbi_image_free(img2);







/*cl_int err = 0;

std::vector<int> vec(1024);
std::fill(vec.begin(), vec.end(), 1); //CPU

cl::Buffer inBuf(context, CL_MEM_READ_ONLY | CL_MEM_HOST_NO_ACCESS | CL_MEM_COPY_HOST_PTR, sizeof(int)*vec.size(), vec.data(), &err);
cl::Buffer outBuf(context, CL_MEM_WRITE_ONLY | CL_MEM_HOST_READ_ONLY, sizeof(int)*vec.size(), nullptr, &err);
cl::Kernel kernel(program, "ProcessArray");
err = kernel.setArg(0, inBuf);
err = kernel.setArg(1, outBuf);

cl::CommandQueue queue(context, device);
//err = queue.enqueueFillBuffer(inBuf, 3, sizeof(int)*10, sizeof(int)*(vec.size()-10));//preenche o buffer

queue.enqueueNDRangeKernel(kernel, cl::NullRange, cl::NDRange(vec.size()));
queue.enqueueReadBuffer(outBuf, CL_FALSE, 0, sizeof(int)*vec.size(), vec.data());

cl::finish();

std::cin.get();*/