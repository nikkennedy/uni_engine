/*	--------------------------------------------------------------------------------- 
	Filename      - hlight.cpp
	Last Modified - 20/04/03
	Author        - Nicholas Kennedy
	Project       - Horizon (3rd Year BSC Computer Science Project "Interactive Game 
			        system with Intelligence")
	Description   - Front end to the lightmap creation process
	--------------------------------------------------------------------------------- */

#include <stdio.h>
#include <windows.h>
#include <io.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <types.h>
#include <debug.h>
#include <parse.h>
#include <maths.h>
#include <lbsp.h>
#include <lightmap.h>
#include <lightmap_gen.h>

// --------------------------------------------------------------------------------- 
// main
// --------------------------------------------------------------------------------- 
int main(int argc, char **argv)
{
#ifdef DEBUG
	c_debug = new CDebug();
	c_debug->debug_init("hlight.dat");
	dbp("hLight Debug Started\n");
#endif
	
	printf("\nLightMap Generator by Nicholas Kennedy (C) 2003\n\n");

	if(argc < 4) {
		printf("Usage: <infile.bsp> <outfile.lbsp> <pathforlightmaps>\n\n");
		printf(" If you place a one at the end of the command line, low-res\n");
		printf(" lightmaps will be used.\n\n");
		return 0;
	}

	CLightMapGenerator c_lmg = CLightMapGenerator();

	if(argc == 4) {
		c_lmg.set_high_res();
	}

	printf("Generating all the light maps (this may take some time!)...\n");
	c_lmg.generate_light_maps(argv[1], argv[2], argv[3]);

	return 1;
}

// END OF FILE ------------------------------------------------------------------------