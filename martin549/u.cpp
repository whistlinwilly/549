#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <dirent.h>
#include <iostream>
#include <fstream>

using namespace std;

int main (int argc,char** argv)
{
  DIR *dp;
  struct dirent *ep;
  char buffer[1024];
  

  if (argc != 2){
    printf("%d\n",42);
  }

  if (!strcmp(argv[1],"0")){
    cout << "resizing image..." << endl;
    dp = opendir("./Test/Pos");
    if (dp != NULL)
    {
      while (ep = readdir (dp)) {
	if (strlen(ep->d_name) > 4) {

	  strcpy(buffer,"./window ");
	  strcat (buffer,"./Test/Pos/");
	  strcat(buffer, ep->d_name);
	  cout << "resizing " << (ep->d_name) << endl;
	  int ret = system(buffer);
	}
      }
      cout << "closing folder" << endl;
      (void) closedir (dp);
      cout << "closed" << endl;
    }
    else
      perror ("Couldn't open the directory");
    
  }
  
  
  if (!strcmp(argv[1],"1")){
    cout << "resizing image..." << endl;
    
    dp = opendir ("./Test/Neg");
    if (dp != NULL)
      {
	while (ep = readdir (dp)) {
	if (strlen(ep->d_name) > 4) {
	  strcpy(buffer,"./window ");
	  strcat (buffer,"./Test/Neg/");
	  strcat(buffer, ep->d_name);
	  cout << "resizing " << (ep->d_name) << endl;
	  
	  int ret = system(buffer);
	}
	}
	cout << "closing folder" << endl;
	(void) closedir (dp);
	cout << "closed" << endl;
	
      }
    else
      perror ("Couldn't open the directory");

  }
  
  
  if (!strcmp(argv[1],"2")){    

    dp = opendir ("./Test/Pos");
    if (dp != NULL)
      {
	while (ep = readdir (dp)) {
	  if (strlen(ep->d_name) > 4) {
	    strcpy (buffer,"./project ");
	    strcat (buffer,"./Test/Pos/");
	    strcat(buffer, ep->d_name);
	    strcat(buffer, " 1");
	    int ret = system(buffer);
	    cout << "Writing HOG of " << (ep->d_name) << " to HOG_file.dat" << endl; 
	  }
	}
	cout << "closing folder" << endl;
	(void) closedir (dp);
	cout << "closed" << endl;

      }
    else
      perror ("Couldn't open the directory");
    
  }
  

  
  if (!strcmp(argv[1],"3")){    
    cout << "clearing previous data..." << endl;
    ofstream myfile;




    dp = opendir ("./Test/Neg");
    if (dp != NULL)
    {
      while (ep = readdir (dp)) {
	if (strlen(ep->d_name) > 4) {
	  strcpy (buffer,"./project ");
	  strcat (buffer,"./Test/Neg/");
	  strcat(buffer, ep->d_name);
         strcat(buffer, " -1");
	  int ret = system(buffer);
	  cout << "Writing HOG of " << (ep->d_name) << " to HOG_file.dat" << endl;
	}
      }
cout << "closing folder" << endl;
      (void) closedir (dp);
 cout << "closed" << endl;

    }
  else
    perror ("Couldn't open the directory");

  }

 cout << "Done, have a nice day :)" << endl;
  return 0;
}
