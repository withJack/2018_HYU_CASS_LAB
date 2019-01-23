//bitcode_saver.cpp
#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <set>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/ExecutionEngine/ExecutionEngine.h"
#include "llvm/ExecutionEngine/MCJIT.h"
#include "llvm-c/Target.h"
#include "llvm/Support/TargetRegistry.h"
#include "llvm/Support/FileSystem.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/Bitcode/BitcodeReader.h"
#include "llvm/Bitcode/BitcodeWriter.h"

using namespace std;
using namespace llvm;

bool exist_check( std::string file_name )
{
	ifstream f( file_name.c_str() );
	return f.good();
}

int main(int argc, char* argv[])
{
	//argument check
	if( argc < 2 )
	{
		cout << "Error: OpenCLPF file_name" << endl;
		exit(-1);
	}

	//extract extension of file anme
	std::string full_file_name_str = std::string( argv[1] );
	size_t last_dot_pos = full_file_name_str.rfind(".");
	std::string ext = full_file_name_str.substr( last_dot_pos + 1 );
	std::string file_name_str = full_file_name_str.substr( 0, last_dot_pos );

	if( ext == "bc" )
	{
		//bitcode file
	}
	else if( ext == "ll" )
	{
		//IR file
		pid_t pid;

		if( ( pid = fork() ) == 0 )
		{
			//child case
			std::string clang_argv[3];

			//compile using clang
			clang_argv[0] = "llvm-as";
			full_file_name_str = "/tmp/" + file_name_str + ".bc";
			clang_argv[1] = string("-o") + string("=") + full_file_name_str;
			clang_argv[2] = argv[1];

			//casting for execvp
			char* clang_argv_cstr[3];

			for( int i=0; i<3; ++i )
			{
				clang_argv_cstr[i] = const_cast<char*>(clang_argv[i].c_str());
			}
			clang_argv_cstr[3] = (char*) 0;

			//remove previous file
			remove( full_file_name_str.c_str() );
			
			//llvm assemble ll file
			execvp( clang_argv_cstr[0], clang_argv_cstr );

			cerr << "Error: exec is not executed" << endl;
			exit(-1);
		}
		else
		{
			//parent
			int status;
			int wait_ret;
			wait_ret = waitpid( pid, &status, 0);
			
			if( WEXITSTATUS(status) != 0 )
			{
				cerr << "Error: llvm-as ll file error occurs" << endl;
			}

			full_file_name_str = "/tmp/" + file_name_str + ".bc";
		}
	}
	else if( ext == "cl" )
	{
		//cl file
		pid_t pid;

		if( ( pid = fork() ) == 0 )
		{
			//child case
			std::string clang_argv[14];

			//compile using clang
			clang_argv[0] = "clang++";
			clang_argv[1] = "-emit-llvm";
			clang_argv[2] = "-c";
			clang_argv[3] = "-target";
			clang_argv[4] = "-nvptx64--nvidiacl";
			clang_argv[5] = "-Dcl_clang_storage_class_specifiers";
			clang_argv[6] = "-isystem";
			clang_argv[7] = "/generic/include";
			clang_argv[8] = "-include";
			clang_argv[9] = "/usr/local/include/clc/clc.h";
			clang_argv[10] = "-fpack-struct=64";
			clang_argv[11] = "-o";
			full_file_name_str = "/tmp/" + file_name_str + ".bc";
			clang_argv[12] = full_file_name_str;
			clang_argv[13] = argv[1];

			//casting for execvp
			char* clang_argv_cstr[15];

			for( int i=0; i<14; ++i )
			{
				clang_argv_cstr[i] = const_cast<char*>(clang_argv[i].c_str());
			}
			clang_argv_cstr[15] = (char*) 0;

			//remove previous file
			remove( full_file_name_str.c_str() );

			//compile opencl file
			execvp( clang_argv_cstr[0], clang_argv_cstr );

			cerr << "Error: exec is not executed" << endl;
			exit(-1);
		}
		else
		{
			//parent
			int status;
			int wait_ret;
			wait_ret = waitpid( pid, &status, 0);
			
			if( WEXITSTATUS(status) != 0 )
			{
				cerr << "Error: compile cl file error occurs" << endl;
			}

			full_file_name_str = "/tmp/" + file_name_str + ".bc";
		}
	}
	else
	{
		cerr << "Error: Undefiend File Format: " << ext << endl;
		exit(-1);
	}


	//input file stream, open from argument( program inputs )	
	ifstream bit_code_file(full_file_name_str); 

	if( bit_code_file.fail() )
	{
		//file open failed
		cout << "Error: File Open " << argv[1] << " Is Failed" << endl;
		exit(-1);
	}

	//entire file read
	string bit_code_file_contents( (istreambuf_iterator<char>(bit_code_file) ), (istreambuf_iterator<char>()) ) ; //i don't know but internet teach me this way

	//prepare to parse bitcode file
	MemoryBufferRef bit_code_file_contents_mbref = MemoryBufferRef( StringRef( bit_code_file_contents), "bitcode_membuf" );
	LLVMContext context;
	Module* module;

	//parse bitcode file
	auto expected_unq_ptr_module = parseBitcodeFile( bit_code_file_contents_mbref, context );
	if( !expected_unq_ptr_module )
	{
		cout << "Error: Parse Bitcode File Error: " << full_file_name_str << endl;
		exit(-1);
	}
	else
	{
		//get module
		module = expected_unq_ptr_module.get().get();

		//wirte module to llvm::raw_ostream( llvm:raw_strring_ostream)
		string bc_string;
		raw_string_ostream bc_str_ostream(bc_string);
		WriteBitcodeToFile( module, bc_str_ostream);
		bc_str_ostream.flush();

		//write to new file
		ofstream output_file(file_name_str + ".updated.bc");
		output_file << bc_string;
		cout << "Write Clear!" << endl;
	}

	return 0;
}
