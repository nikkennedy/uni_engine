-------------------------------------------------------------------------------------------
README.TXT - Nicholas Kennedy 26/04/03 
-------------------------------------------------------------------------------------------

Contained on this CD are the files relating to my third year BSC Computer Science project.
I verify that I am the sole author of these files unless explicitly stated otherwise.

The directory \HORIZON contains three sub-directories

	\code       - the complete source code for this project.
        \executable - a ready to run version of the project.
	\tools      - the pre-compiled offline tools used to create the game levels.
	\dlls       - small variations of the rendering and A.I. DLLs.
                      

The directory \"MD2 Models" contains the original zipped versions of the MD2 models 
used by this project. These are present as requested by their respective authors.


The directory \"3Ds Max Models" contains four sub-directories

	\collectables   - the 3D Studio max models of each collectable.
	\environments   - the 3D studio max models of each environment.
	\rocket	        - the 3D Studio max model of a rocket.
	\"exported ase" - the ASE files for each of the above models exported using
                          3D studio project.

The directory \directx9 contains the redistributable version of Microsoft DirectX v9
which is required by this project.

The directory \"OpenGL SDK" contains nVidia's OpenGL Software Development Kit. This must
be installed in order to compile the source code.

Two batch files are present in the root of this CD, they will convert a .ASE file into
files which are usable by this project.

	convert.bat <asefilename> <outputpath for tempfiles> <outputpath for lightmaps>

                   - converts an ase file in \"3DS Max Models\exported ase" 

	convertall.bat (need to open and change the relevant paths)

                   - converts the four models created for this project.

-------------------------------------------------------------------------------------------


