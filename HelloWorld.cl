__kernel void ProcessArray(__global unsigned char* data, __global unsigned char* outData)
{
	outData [get_global_id(0)] = (0.0722f * data[3 * get_global_id(0)]) 
		+ (0.7152f * data[3 * get_global_id(0) + 1]) 
		+ (0.2126f * data[3 * get_global_id(0) + 2]);
}