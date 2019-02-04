#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

using namespace std;
string processString(string opString);
string getRegisterName(string regHex);


int main(int argc, char *argv[]){
	//TODO: This is a test line
	//cout << "You have selected " << argv[1] << " as your file\n";
	
	ifstream inf(argv[1]);

	if(!inf){
		cout << argv[1] << "  Couldn't be open for reading";
		return 1;
	}else{
		while(inf){
			string strInput;
			getline(inf, strInput);
			if(strInput.size() > 0){
				stringstream ss;
				//ss.str(strInput);
				//TODO: This is a test line
				//cout << ss.str() << endl;

				//string hexAddr;
				//hexAddr = ss >> hexAddr;
				//string opString = ss >> opString;
				//cout << "HexAddr: " << hexAddr << " opString: " << opString << endl;
			

				ss << strInput;
				string hexAddr;
				ss >> hexAddr;
				string operation;
				ss >> operation;
				//cout << hexAddr << "   " << operation << "\n";
				cout << hexAddr << "    " << processString(operation) << "\n";
			}
		}	
	}
	return 0;
}

string processString(string opString){
	//TODO: Investigate optimization of hex vs. string use here
	stringstream str;
	string retString;
	str << opString.substr(0,1);
	int value;
	str >> hex >> value;
	if(opString.size() == 16){
		
	}else{
		switch(value){
			case 0x0 :
				//halt
				retString = "HALT";
				break;
			case 0x1 :
				//nop
				retString = "NOP";
				break;
			case 0x2 :
				//ret
				retString = "RRMOVQ/CMOVXX";
				break;
			case 0x3 :
				//halt
				retString = "IRMOVQ";
				break;
			case 0x4 :
				//nop
				retString = "RMMOVQ";
				break;
			case 0x5 :
				//ret
				retString = "MRMOVQ";
				break;
			case 0x6 :
				//halt
				retString = "OPQ";
				break;
			case 0x7 :
				//nop
				retString = "JXX";
				break;
			case 0x8 :
				//ret
				retString = "CALL";
				break;
			case 0x9 :
				//ret
				retString = "RET";
				break;
			case 0xa :
				//ret
				retString = "PUSHQ";
				break;
			case 0xb :
				//ret
				retString = "POPQ";
				break;
		}
	}
	if(opString.size() == 16){
		retString = "QUAD";
	}
	//TODO: Remove TEST CODE
	return "opstring: " + opString  + " " + retString;
}

string getRegisterName(string regHex){
	
}
