==============================
A. Build and Execution Command
==============================

1. build approximator
	$ make

2. build example
	$ make -C ./2DCONV

3. run exmample
	$ cd ./2DCONV

	$ cp kernel.bin.0 kernel.bin
	$ ./2DConvolution.exe

	$ cp kernel.bin.1 kernel.bin
	$ ./2DConvolution.exe


==============================
B. Description
==============================

approximator.cpp / approximator : implemenation of approximator( load replacer )

	- usage : ./approximator <kernel_source_path> <output_file_path>
	- output : 
		- output_file_path.0 : original binary
		- output_file_path.1 : replace 2nd load to 1st load
		- output_file_path.2 : replace 3rd load to 1st load ( 2nd already replaced )
		- output_fiel_path.3 : replace 4th load to 1st load ( 2nd, 3rd already replaced )
