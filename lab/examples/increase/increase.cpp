//insert printf.cpp
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

		// "Hello World" global string
		std::string hello_world_string = "(TEST)Number of Instruction Executed: %d\n";
		Constant *constant_hello_world = ConstantDataArray::getString( context, hello_world_string );
		GlobalVariable* gvar_hello_world = new GlobalVariable(
				(*module),
				constant_hello_world->getType(),
				true,
				GlobalVariable::PrivateLinkage,
				NULL,
				Twine( "hello_world_string" )
				);
		gvar_hello_world->setAlignment(1);
		gvar_hello_world->setInitializer( constant_hello_world );

		// declare function prototype: printf
		Function *printf_func = module->getFunction("printf");
		if(printf_func == NULL )
		{
			//undeclared printf functions
			std::vector<Type*> printf_params_type;
			printf_params_type.push_back( PointerType::get(IntegerType::get(context, 8), 0) );
			FunctionType *printf_type = FunctionType::get(IntegerType::get(context, 32), printf_params_type, true);
			Function *printf_func = Function::Create(printf_type, Function::ExternalLinkage, "printf", module); 

			printf_func->setCallingConv(CallingConv::C);
		}

		//Builder
		IRBuilder<> Builder(context);

		//get main function
		Function* main_func = module->getFunction("main");
		if(main_func == NULL)
		{
			cerr << "There is no Main Functions Here" << endl;
			exit(-1);
		}


		//get origianl instruction pointer
		vector<Instruction*> inst_vec;
		for( Function::iterator func_iter = main_func->begin(); func_iter != main_func->end(); ++func_iter)
		{
			for( BasicBlock::iterator bb_iter = func_iter->begin(); bb_iter != func_iter->end(); ++bb_iter)
			{
				inst_vec.push_back( &(*bb_iter));
			}
		}
		
		//set insert point to first instruction
		Builder.SetInsertPoint( &(*(main_func->begin()->begin())) );

		//alloca i32
		AllocaInst* inc_var_alloca = Builder.CreateAlloca(IntegerType::get(context, 32), nullptr, "alloca.variable");
		Builder.CreateStore( llvm::Constant::getNullValue(IntegerType::get(context, 32)) , inc_var_alloca);

		for( int i=0, Size = inst_vec.size(); i<Size; ++i)
		{
			//ser insert point to each instruction
			Builder.SetInsertPoint( inst_vec[i] );

			//load variable
			Value* load_var = Builder.CreateLoad(inc_var_alloca, "load.variable");
			
			//inc variable(++)
			Value* add_var = Builder.CreateAdd(load_var, ConstantInt::get( IntegerType::get(context, 32), 1, false), "add.variable");

			//store variable
			Builder.CreateStore(add_var, inc_var_alloca);

			if( inst_vec[i]->isTerminator() )
			{
				//printf call(just for test)
				std::vector<Value*> printf_arguments;
				Constant *zero = Constant::getNullValue( IntegerType::get(context, 32) );
				Constant *indices[] = {zero, zero};
				Value* elementPtr_gvar_hello_world = ConstantExpr::getGetElementPtr(
						cast<PointerType>(gvar_hello_world->getType()->getScalarType())->getContainedType(0u),
						(Constant*)gvar_hello_world, 
						ArrayRef<Constant*>(indices), 
						true
						);
				Value * inc_var_load = Builder.CreateLoad(inc_var_alloca);
				printf_arguments.clear();
				printf_arguments.push_back(elementPtr_gvar_hello_world);
				printf_arguments.push_back(inc_var_load);
				Builder.CreateCall( printf_func, printf_arguments, "printf_call_tmp" );
			}
		}

		//wirte module to llvm::raw_ostream( llvm:raw_strring_ostream)
		string bc_string;
		raw_string_ostream bc_str_ostream(bc_string);
		WriteBitcodeToFile( module, bc_str_ostream);
		bc_str_ostream.flush();

		//write to new file
		ofstream output_file(file_name_str + ".updated.bc");
		output_file << bc_string;
		cout << "Write Clear!" << endl;
		cout << "Number_of_Instruction(Static Check) : " << inst_vec.size() << endl;
	}

	return 0;
}
