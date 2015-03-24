#include <set>
#include <ctime>
#include <string>
#include <vector>
#include <memory>
#include <cassert>
#include <cstdint>
#include <cassert>
#include <fstream>
#include <iterator>
#include <iostream>
#include <algorithm>
#include <stdexcept>
#include <sstream>

#include <tclap/CmdLine.h>

#include "test.h"
#include "vertexenumerator.h"

size_t Atoi(const char * str)
{
	size_t ret;
	std::stringstream ss(str);
	ss >> ret;
	return ret;
}

int main(int argc, char * argv[])
{
	//assert(Sibelia::Runtests());
	
	try
	{
		TCLAP::CmdLine cmd("Program for condensed de Bruijn graph construction", ' ', "0");
		
		TCLAP::SwitchArg countAll("",
			"all",
			"Count all bifurcation",
			cmd,
			false);

		TCLAP::ValueArg<unsigned int> kvalue("k",
			"kvalue",
			"Value of k",
			false,
			25,
			"integer",
			cmd);

		TCLAP::ValueArg<uint64_t> filterSize("f",
			"filtersize",
			"Size of the filter",
			true,
			0,
			"integer",
			cmd);

		TCLAP::ValueArg<unsigned int> hashFunctions("q",
			"hashfnumber",
			"Number of hash functions",
			false,
			5,
			"integer",
			cmd);

		TCLAP::ValueArg<unsigned int> rounds("r",
			"rounds",
			"Number of hash functions",
			false,
			1,
			"integer",
			cmd);

		TCLAP::ValueArg<unsigned int> threads("t",
			"threads",
			"Number of hash functions",
			false,
			1,
			"integer",
			cmd);

		TCLAP::UnlabeledMultiArg<std::string> fileName("filenames",
			"FASTA file(s) with nucleotide sequences.",
			true,
			"fasta files with genomes",
			cmd);


		cmd.parse(argc, argv);
		
		Sibelia::VertexEnumerator vid(fileName.getValue(), kvalue.getValue(), filterSize.getValue(), hashFunctions.getValue(), rounds.getValue(), threads.getValue());
		std::cout << "Distinct = " << vid.GetVerticesCount() << std::endl;

		if (countAll.isSet())
		{
			std::vector<std::string> all;
			vid.Dump(std::back_inserter(all));
			std::vector<std::string> rev;
			std::transform(all.begin(), all.end(), std::back_inserter(rev), static_cast<std::string(*)(const std::string&)>(Sibelia::DnaString::RevComp));
			all.insert(all.begin(), rev.begin(), rev.end());
			std::sort(all.begin(), all.end());
			all.erase(std::unique(all.begin(), all.end()), all.end());
			std::cout << "All = " << all.size() << std::endl;
		}

		std::cout << std::endl;

	}
	catch (TCLAP::ArgException &e)
	{
		std::cerr << "error: " << e.error() << " for arg " << e.argId() << std::endl;
		return 1;
	}
	catch (std::runtime_error & e)
	{
		std::cerr << "error: " << e.what() << std::endl;
		return 1;
	}

	

	return 0;
}