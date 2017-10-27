#include <cstdio>
#include <dirent.h>
#include <sys/types.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

using namespace std;

#define OFFSET_PE 0x3C
#define PE 0x4550
#define X86 0x14c
#define X64 0x8664


string operator*(string s, size_t count)
{
  string ret;
  for(size_t i = 0; i < count; ++i)
  {
    ret = ret + s;
  }
  return ret;
}

int checkDirectory(const char * dir_name,const char * path_name)
{
  DIR* path = opendir(path_name);
  if (dir_name[0] == '.')
  {
    return -1;
    
  }
  else if(path == NULL)
  {
    return 0;
  } else if (path)
  {
    closedir(path);
    return 1;
  }
  else{
    return -1;
  }
}

void DirectoryEnumeration(const char * dir_name, int count)
{
  DIR* path = opendir(dir_name);
  unsigned char address;
  unsigned short int checkPE, checkBit;
  if(!path)
  {
    fprintf (stderr, "Cannot open directory '%s': %s\n",
             dir_name, strerror (errno));
    return;
  }
  int a = 0;;
  struct dirent * directoryStruct;
  while(1)
  {
    directoryStruct = readdir(path);
    if (directoryStruct == NULL){
      closedir(path);
      return;
    }
    stringstream ss;
    ss << dir_name <<"/"<< directoryStruct->d_name;
    const string s = ss.str();
    const char* str = s.c_str();
    string space(" ");
    cout << space*count << directoryStruct->d_name;
    a = checkDirectory(directoryStruct->d_name, str);
    if(a == 1)
    {
      cout << endl;
      DirectoryEnumeration(str, count + 5);
    }
    else if (a == -1){
      cout << endl;
      continue;
    } else
    {
      FILE * file;
      if ((file = fopen(str, "r")) == NULL){
        cout << "cant"<<endl;
        continue;
      }
      fseek(file, OFFSET_PE, SEEK_SET);
      fread(&address, 1, 1, file);
      fseek(file, address, SEEK_SET);
      fread(&checkPE, 2, 1, file);
      if(checkPE == PE)
      {
        cout << " PE ";
        fseek(file, 2, SEEK_CUR);
        fread(&checkBit, 2, 1, file);
        if(checkBit == X86)
        {
          cout << "32 ";
        } else if(checkBit == X64)
        {
          cout << "64 ";
        } else
        {
          cout << "???";
        }
      } else
      {
        cout << " NOT PE ";
      }
      fclose(file);
    }
    cout << endl;
  }
  
}

int main (int argc, char* argv[])
{
  DirectoryEnumeration("/Users/mak/Desktop", 0);
  return 0;
}
