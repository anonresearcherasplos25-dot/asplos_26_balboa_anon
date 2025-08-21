// Anonymous contribution
#include <iostream>
#include <random>
#include <string>
#include <fstream>
#include <math.h>


//Default values
//block size: 2
int main(int argc, char *argv[])
{
   if (argc != 5)
   {
      std::cerr << "ERROR number of arguments not correct\n";
      std::cerr << "Usage generate_random_table <number of tables> <block size> <length of key> <length of hash>\n";

      return -1;
   }

   int numTables = atoi(argv[1]);
   int blockSize = atoi(argv[2]);
   int keyLength = atoi(argv[3]);
   int hashLength = atoi(argv[4]);
   std::string fileName("tabulation_table.txt");
   std::ofstream outfile;
   outfile.open(fileName.c_str());
   if(!outfile)
   {
      std::cerr << "Could not open output file";
      return -1;
   }

   uint32_t maxKeyValue = pow(keyLength, 2) - 1;
   std::default_random_engine generator;
   std::uniform_int_distribution<int> distribution(0, maxKeyValue);

   for (int i = 0; i < numTables; i++)
   {
      outfile << "{";
      for (int j = 0; j < blockSize; j++)
      {
         outfile << "{";
         for (int k = 0; k < keyLength; k++)
         {
            int randomNumber = distribution(generator);
            outfile << randomNumber;
            if (k < keyLength-1)
               outfile << ",";
         }
         outfile << "}";
         if (j < blockSize-1)
            outfile << ",";
      }
      outfile << "}";
      if (i < numTables-1)
         outfile << "," << std::endl;
   }

   outfile.close();
   std::cout << "Table succesfully generated" << std::endl;
   return 0;
}
