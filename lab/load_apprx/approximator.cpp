#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdio>
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Metadata.h"
#include "llvm/IR/Instruction.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Function.h"
#include "llvm/Bitcode/BitcodeReader.h"
#include "llvm/Bitcode/BitcodeWriter.h"
#include "llvm/Transforms/Utils/ValueMapper.h"
#include "llvm/Transforms/Utils/Cloning.h"
#include "llvm/Transforms/Utils/BasicBlockUtils.h"
#include "llvm/Support/raw_os_ostream.h"

/////////////////////////
// Macros
////////////////////////
#define QUOTE( X ) #X
#define STRING_OF_VALUE( X ) QUOTE( X )

#ifdef LLVM_PATH
#define LLVM_PATH_STRING std::string( STRING_OF_VALUE( LLVM_PATH ) )
#else
#define LLVM_PATH_STRING "/usr/local/llvm"
#endif 

#ifdef LIBCLC_PATH
#define LIBCLC_PATH_STRING std::string( STRING_OF_VALUE( LIBCLC_PATH ) )
#else
#define LIBCLC_PATH_STRING "/usr/local/libclc"
#endif


////////////////////////
// Global Variables
///////////////////////
llvm::LLVMContext* TheContext;
std::unique_ptr< llvm::Module > TheModule;
llvm::IRBuilder<>* TheBuilder;
std::string kernel_source_path;
std::string output_file_path;		// 2DConvolution.cl
std::string working_file_path;		// kernel.bin
std::vector< std::string > temporary_files; // 자료구조: std::vector

///////////////////////
// Functions
///////////////////////
void print_usage( void );
void parse_argument( int argc, char* argv[] );
void compile_source_code( void );
void get_module( void );
void module_writing( void );
void backend_process( void );
void remove_temporary_files( void );

int get_num_load( void );
void replace_load( int source, int target );

/////////////////
// MAIN Functions
/////////////////
int main( int argc, char* argv[] )
{
	parse_argument( argc, argv );
	compile_source_code();
	get_module();
	
	int num = get_num_load();
	printf("%d\n", num);
	
	// iteration for # of load instructions.
//	for( int i=0,Size=get_num_load(); i<Size; ++i )
	for( int i=1,Size=2; i<Size; ++i )
	{
		std::string original_output_file_path = output_file_path;
		output_file_path += "." + std::to_string( i );		// e.g. kernel.bin.0

		// replace i instructions with the first load instruction.
		replace_load( 0, i );
		module_writing();
		backend_process();

		output_file_path = original_output_file_path;	// kernel.bin
	}
	remove_temporary_files( );

	return 0;
}

///////////////////////////
// Function Impementations
///////////////////////////

//print usage
void print_usage( void )
{
	std::cout << "approximator kernel_source_path output_file_path" << std::endl;
}

//initialize global variables with command arguments
void parse_argument( int argc, char* argv[] )
{
	if( argc != 3 )
	{
		std::cerr <<"["<<__FILE__<<":"<<__LINE__<<"] "<< "# of Argument is not 3" << std::endl;
		print_usage();
		exit(-1);
	}

	temporary_files.clear();
	TheModule.reset();
	TheBuilder = nullptr;
	TheContext = nullptr;
	kernel_source_path = std::string( argv[1] );		// 2DConvolution.cl
	output_file_path = std::string( argv[2] );			// kernel.bin
	working_file_path = kernel_source_path;
}

//compile source code(opencl kernel) to llvm IR
void compile_source_code( void )
{
	std::string temp_bitcode_path = "__kernel.temp.bc";
	std::string compile_command = 
		LLVM_PATH_STRING + "/bin/clang " 
		+ "-emit-llvm -c "
		+ "-target -nvptx64-nvidia-nvcl "
		+ "-Dcl_clang_storage_class_specifiers "
		+ "-include " + LIBCLC_PATH_STRING + "/include/clc/clc.h "
		+ "-I " + LIBCLC_PATH_STRING + "/include/ "
		+ "-fpack-struct=64 -O3 "
		+ "-o " + temp_bitcode_path + " "
		+ working_file_path;

	system( compile_command.c_str() );
	working_file_path = temp_bitcode_path;
	temporary_files.push_back( temp_bitcode_path );
}

//load module from bitcode file
void get_module( void )
{
	//read bitcode file
	std::string kernel_bitcode_contents;
	std::ifstream kernel_bitcode_ifstream( working_file_path );
	if( kernel_bitcode_ifstream.fail() )
	{
		std::cerr <<"["<<__FILE__<<":"<<__LINE__<<"] "<< "File Read Failed : " << working_file_path << std::endl;
		exit( -1 );
	}
	kernel_bitcode_contents.assign(
			(std::istreambuf_iterator< char >( kernel_bitcode_ifstream ) ),
			(std::istreambuf_iterator< char >( )) 
			);
	kernel_bitcode_ifstream.close();

	//theContext allocation
	TheContext = new llvm::LLVMContext();

	//parse bitcode file
	llvm::MemoryBufferRef kernel_bitcode_contents_mbref( llvm::StringRef( kernel_bitcode_contents ), "kernel_membuf" );
	auto expected_unq_ptr_module = llvm::parseBitcodeFile( kernel_bitcode_contents_mbref, *TheContext );

	if( !expected_unq_ptr_module )
	{
		std::cerr <<"["<<__FILE__<<":"<<__LINE__<<"] "<< "Parse Bitcode File Failed" << std::endl;
		exit( -1 );
	}

	TheModule = std::move(expected_unq_ptr_module.get());
}

//print loaded module to bitcode file
void module_writing( void )
{
	llvm::raw_os_ostream raw_cout( std::cout );
	TheModule->print(raw_cout, nullptr);

	//print loaded( modified ) module to std::string
	std::string bitcode_string;
	llvm::raw_string_ostream bitcode_string_ostream( bitcode_string );
	llvm::WriteBitcodeToFile( TheModule.get(), bitcode_string_ostream );
	bitcode_string_ostream.flush();


	//print out bitcode string
	std::string output_bitcode_path = "__kernel.temp.processed.bc";
	std::ofstream output_file_ofstream( output_bitcode_path );
	if( !output_file_ofstream.good() )
	{
		std::cerr <<"["<<__FILE__<<":"<<__LINE__<<"] "<< "File Open Failed" << std::endl;
		exit( -1 );
	}

	output_file_ofstream << bitcode_string;
	output_file_ofstream.close();

	temporary_files.push_back( output_bitcode_path );
	working_file_path = output_bitcode_path;
}

//backend process of bitcode file
// output_file_path = "kernel.bin.#"
void backend_process( void )
{
	//IR-level link with libclc libs
	std::string linked_bitcode_path = "__kernel.linked.bc";
	std::string ir_link_command = 
		LLVM_PATH_STRING + "/bin/llvm-link " 
		+ "-o " + linked_bitcode_path + " "
		+ working_file_path + " "
		+ LIBCLC_PATH_STRING + "/lib/clc/nvptx64--nvidiacl.bc";

	system( ir_link_command.c_str() );

	temporary_files.push_back( linked_bitcode_path );
	working_file_path = linked_bitcode_path;

	//llvm static compile
	std::string static_compile_command =
		LLVM_PATH_STRING + "/bin/llc "
		+ "-mcpu=sm_20 -march=nvptx64 "
		+ "-o " + output_file_path + " "
		+ working_file_path;

	system( static_compile_command.c_str() );
	working_file_path = output_file_path;
}

//remove temporary files
void remove_temporary_files( void )
{
	for( int i=0, Size=temporary_files.size(); i<Size; ++i )
	{
		std::string temporary_file_path = temporary_files[i];
		remove( temporary_file_path.c_str() );
	}
}


//get number of load instructions in module
// module: 파일단위
// function: 함수 단위
// BasicBlock: 한번에 jump없이 실행되는 block단위분. label로 구분
// instruction: 명령어 단위
int get_num_load( void )
{
	int number_of_load = 0;

	//a module has list of function
	for( llvm::Module::iterator ModIter = TheModule->begin();
			ModIter != TheModule->end();
			++ModIter )
	{
		//llvm::Module::iteartor == llvm::Function*
		llvm::Function *Func = llvm::cast< llvm::Function >( ModIter );
		//a function has list of basicblocks
		for( llvm::Function::iterator FuncIter = Func->begin();
				FuncIter != Func->end();
				++FuncIter )
		{
			//llvm::Function::iterator == llvm::BasicBlock*
			llvm::BasicBlock *BB = llvm::cast< llvm::BasicBlock >( FuncIter );

			//a basicblock has list of instructions
			for( llvm::BasicBlock::iterator BBIter = BB->begin();
					BBIter != BB->end(); 
					++BBIter )
			{
				//llvm::BasicBlock::iterator == llvm::Instruction*
				llvm::Instruction* Inst = llvm::cast< llvm::Instruction >( BBIter );


				//check if it is load instruction
				if( llvm::isa< llvm::LoadInst >( Inst ) )
				{
					number_of_load++;
				}
			}
		}
	}

	//return
	return number_of_load;
}

//replace load instructions
// find load instructions with same method used in get_num_load() 
// and push it into vector.
// then, replace the target instruction with the very first load instruction.
void replace_load( int source, int target )			// source: 0, target: i(load instruction #)
{
	//get load instruction
	static std::vector< llvm::LoadInst* > load_inst_vector;

	// get load instruction and push it into load_inst_vector.
	if( load_inst_vector.empty() )
	{
		//a module has list of function
		for( llvm::Module::iterator ModIter = TheModule->begin(); 
				ModIter != TheModule->end(); 
				++ModIter )
		{
			//llvm::Module::iteartor == llvm::Function*
			llvm::Function *Func = llvm::cast< llvm::Function >( ModIter );
			//a function has list of basicblocks
			for( llvm::Function::iterator FuncIter = Func->begin();
					FuncIter != Func->end();
					++FuncIter )
			{
				//llvm::Function::iterator == llvm::BasicBlock*
				llvm::BasicBlock *BB = llvm::cast< llvm::BasicBlock >( FuncIter );

				//a basicblock has list of instructions
				for( llvm::BasicBlock::iterator BBIter = BB->begin();
						BBIter != BB->end(); 
						++BBIter )
				{
					//llvm::BasicBlock::iterator == llvm::Instruction*
					llvm::Instruction* Inst = llvm::cast< llvm::Instruction >( BBIter );

					//check is load instruction
					if( llvm::isa< llvm::LoadInst >( Inst ) )
					{
						llvm::LoadInst* lInst = llvm::cast< llvm::LoadInst >( Inst );
						load_inst_vector.push_back( lInst );
					}
				}
			}
		}
	}


	//replace load instructions
	if( source == target ) return;

	llvm::LoadInst *sourceInst = load_inst_vector[source];
	llvm::LoadInst *targetInst = load_inst_vector[target];
//	llvm::errs() << "[%s] " << llvm::cast< llvm::Instruction >(targetInst) << "\n";
//printf("--------------------------------------------------------------------------------\n%s\n================================================================================\n%s\n", (char *) sourceInst, (char *) targetInst);
	targetInst->replaceAllUsesWith( sourceInst ); //replace
//	targetInst->dump();
//printf("********************************************************************************\n%s\n", (char *) targetInst);
	targetInst->dropAllReferences();
	targetInst->removeFromParent(); //remove
}
