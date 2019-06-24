/*	--------------------------------------------------------------------------------- 
	Filename      - main.cpp 
	Last Modified - 20/04/03
	Author        - Nicholas Kennedy
	Project       - Horizon (3rd Year BSC Computer Science Project "Interactive Game 
			        system with Intelligence")
	Description   - Front end to the asetohrz file format convertor
	--------------------------------------------------------------------------------- */

#include <windows.h>
#include <stdio.h>

#include <ase_parse.h>
#include <types.h>

// --------------------------------------------------------------------------------- 
// main - start point of execution
// --------------------------------------------------------------------------------- 
int main(int argc, char **argv)
{
	GeomObject *objects = NULL; // result is a list of objects
	int num_objs;

	printf("\nASEtoHRZ Convertor by Nicholas Kennedy (C) 2003\n\n");

	if(argc < 2) {
		printf("Usage: asetohrz <inputfile.ase> <outputfile.hrz>\n\n");
		return 0;
	}

	printf("Running: turning %s -> %s\n", argv[1], argv[2]);

	CAseParser *ase_parser = new CAseParser();
//	if(argc == 4) {
//		ase_parser->oldversion = 1;
//		printf("This is the OLD version.\n");
//	}

	// --------------------------------------------------------------------------------- 
	// parse the ASE file creating an HRZ representation
	// --------------------------------------------------------------------------------- 
	if(!(num_objs = ase_parser->parse_file(argv[1], objects))) {
		printf("There were problems converting the file.\n");
		return 0;
	}

	// --------------------------------------------------------------------------------- 
	// report some information
	// --------------------------------------------------------------------------------- 
	printf("There where %4d objects.\n", ase_parser->num_objects);
	printf("            %4d lights.\n", ase_parser->num_lightobjects);
	printf("Completed parsing so saving to: %s\n", argv[2]);
	
	
	// --------------------------------------------------------------------------------- 
	// save the created HRZ model
	// --------------------------------------------------------------------------------- 
	ase_parser->output_hrz(argv[2]);	
	printf("Done\n\n");
	return 1;
}

// END OF FILE ----------------------------------------------------------------