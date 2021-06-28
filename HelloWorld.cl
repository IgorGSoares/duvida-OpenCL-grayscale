//__constant sampler_t sampler = CLK_NORMALIZED_COORDS_FALSE | CLK_ADDRESS_CLAMP_TO_EDGE | CLK_FILTER_NEAREST;

__kernel void ProcessArray(__global unsigned char* data, __global unsigned char* outData) //, __global int* canal1, __global int canal2
{
	outData [get_global_id(0)] = (0.0722f * data[3 * get_global_id(0)]) + (0.7152f * data[3 * get_global_id(0) + 1]) + (0.2126f * data[3 * get_global_id(0) + 2]);

	/*data + canal1;
	outData + canal2;
	outData[get_global_id(0)] = (data[get_global_id(0)] + data[get_global_id(0)+1] + data[get_global_id(0)+2])/3.0f;
	if(canal1 == 4)
	{
		outData[get_global_id(0)+1] = data[get_global_id(0)+3];
	}*/
}

/*for (unsigned char *p = background, *pg = gray_img; p != background + img_size; p += channelsB, pg += gray_channels)
	{
		*pg = (uint8_t)((*p + *(p + 1) + *(p + 2)) / 3.0);
		if (channelsB == 4) {
			*(pg + 1) = *(p + 3);
		}
	}*/