
#include <iostream>
#include<string>
#include<sstream>
#include<vector>
#include<math.h>
using namespace std;

char Vn[2] = { 'S', 'A' };
char Vt[4] = { '.', '0', '1', '#' };

/*
（1）S->A.A|A
（2）A->0A|1A
（3）A->0|1
*/

/*
FIRSTVT(S)={ # , . , 0 , 1 }，FIRSTVT(A)={ 0 , 1 }
LASTVT(S)={ # , . , 0 , 1 }，LASTVT(A)={ 0 , 1 }

A.  :  FIRSTVT(A)>.
.A  :  .<FIRSTVT(A)
0A  :  0<FIRSTVT(A)
1A  :  1<FIRSTVT(A)

.        0       1       #
.              <       <       >
0     >        <       <       >
1     >        <       <       >
#     <        <       <       =

*/
int operatorTable[4][4] = { { -999, -1, -1, 1 }, { 1, -1, -1, 1 }, { 1, -1, -1, 1 }, { -1, -1, -1, 0 } };
//-999表示无关系，1表示先于关系，0表示等价关系，-1表示后于关系
void  Analyze();

//定义剩余输入串存放栈
vector<char> stackInputLeft;

//主函数
int main(){

	int i = 0;
	char a[1000];
	cout << "请输入一个无符号二进制数，并以#结束：\n";
	do{
		cin >> a[i];
		i++;
	} while (a[i - 1] != '#');

	for (int j = i - 1; j >= 0; j--){
		stackInputLeft.push_back(a[j]);
	}
	Analyze();
	return 0;
}

void Analyze()
{
	//定义变量info用于输出提示信息
	string info = "";
	cout << "步骤" << "\t\t"
		<< "分析栈" << "\t\t"
		<< "当前输入符" << "\t"
		<< "剩余输入符"<< "\t\t"
		<< "动作" << endl;
	//定义结果初始值为0；
	double result = 0;
	//赋值currentInputChar用于存放当前输入符
	char currentInputChar = ' ';
	//定义firstSymbolChar用于存放符号栈的最外面的一个终结符
	char firstSymbolChar = ' ';

	//定义动作，初始值为“移进”
	string action = "移进";

	//定义符号栈
	vector<char> stackSymbol;
	//符号栈底部为#
	stackSymbol.push_back('#');
	//赋值currentInputChar为输入串的第一个字符
	currentInputChar = stackInputLeft[stackInputLeft.size() - 1];
	//弹出stackInputLeft第一个元素
	stackInputLeft.pop_back();

	bool finish = false;
	int step = 0;


	//定义三个变量用于确定数字小数点左边的位数和小数点右边的位数
	//leftLimit为小数点左边的位数，rightLimit为小数点右边的位数，temp为求左右边界时的临时变量
	//temp1为求小数点左边值的临时变量
	int  rightLimit = 0, temp1 = 0;
	do{
		//求当前分析栈的内容
		string symbolContent = "";
		int size1 = stackSymbol.size() - 1;
		for (int i = 0; i <= size1; i++){
			symbolContent = symbolContent + stackSymbol[i];
		}
		//求当前剩余输入串的内容
		string inputContent = "";
		int size2 = stackInputLeft.size();
		if (size2 == 0){
			inputContent = "";
		}
		else{
			for (int i = size2 - 1; i >= 0; i--){
				inputContent = inputContent + stackInputLeft[i];
			}
		}

		//获取符号栈的最外面一个终结符
		for (int i = stackSymbol.size(); i>0; i--){
			for (int j = 0; j<4; j++){
				if (stackSymbol[i - 1] == Vt[j]){
					//获取到符号栈的最外面的一个终结符，将其赋值给firstSymbolChar；
					firstSymbolChar = Vt[j];
					//跳出俩层循环
					goto breakLoop;
				}
			}
		}

		//定义算法优先级标志priority，初始值为-99999，不等于算符优先关系表中的任一值
		breakLoop:int priority = -99999;
		//定义临时变量x,y用于确定终结符优先关系对应表中的位置
		int x = -1, y = -1;
		for (int i = 0; i<4; i++){

			if (firstSymbolChar == Vt[i]){
				x = i;
			}
			if (currentInputChar == Vt[i]){
				y = i;
			}
		}
		//根据算符优先关系表中的数据重新赋值priority
		if (x >= 0 && x < 4 && y >= 0 && y < 4){
			priority = operatorTable[x][y];
		}

		//根据priority的值是否重新赋值来判断输入串是否包含非法字符
		if (priority == -99999){
			finish = true;
			action = "分析失败";
			stringstream stream;
			stream << "输入串包含非法字符【" << currentInputChar << "】！" << endl;
			info = stream.str();
		}
		//如果重新赋值后的priority值为-999，则俩个终结符无关系，分析失败
		else if (priority == -999){
			finish = true;
			action = "分析失败";
			stringstream stream;
			stream << "输入串不符合文法要求" << endl;
			info = stream.str();
		}
		//如果重新赋值后的priority值为0，则分析成功，结束
		else if (priority == 0){
			finish = true;
			action = "分析成功";
			stringstream stream;
			stream << "将该二进制数转换为十进制数为" << result << "。" << endl;
			info = stream.str();
		}
		//如果重新赋值后的priority值为-1，则移进当前输入符
		else if (priority == -1){
			//移进时，如果要移进的字符为0或1
			if (currentInputChar == '0' || currentInputChar == '1'){
				rightLimit++;
			}
			//如果要移进的字符为.
			else if (currentInputChar == '.'){
				rightLimit = 0;
			}
			//将要移进的符号移进到符号栈
			stackSymbol.push_back(currentInputChar);
			//将剩余输入串栈顶元素赋值给currentInputChar
			currentInputChar = stackInputLeft[stackInputLeft.size() - 1];
			//将剩余输入串的第一个字符出栈
			stackInputLeft.pop_back();
			action = "移进";
		}
		//如果重新赋值后的priority值为1，则执行归约动作
		else if (priority == 1){
			//如果符号栈最顶元素为A,将其弹出
			if (stackSymbol[stackSymbol.size() - 1] == 'A'){
				stackSymbol.pop_back();
			}
			//如果当前输入字符为.
			if (currentInputChar == '.'){
				//如果符号栈最外面的终结符为0
				if (firstSymbolChar == '0'){
					temp1++;
					stackSymbol.pop_back();
					stackSymbol.push_back('A');
				}
				//如果符号栈最外面的终结符为1
				else{
					result += pow(2, temp1);
					temp1++;
					stackSymbol.pop_back();
					stackSymbol.push_back('A');
				}
			}
			//如果当前输入字符为#
			else {
				//如果符号栈最外面的终结符为.
				if (firstSymbolChar == '.'){
					stackSymbol.pop_back();
				}
				//如果符号栈最外面的终结符为0
				else if (firstSymbolChar == '0'){
					if (rightLimit > 0){
						rightLimit--;
						stackSymbol.pop_back();
						stackSymbol.push_back('A');
					}
					else{
						temp1++;
						stackSymbol.pop_back();
						stackSymbol.push_back('A');
					}

				}
				//如果符号栈最外面的终结符为1
				else{
					if (rightLimit > 0){
						result += pow(2, -rightLimit);
						rightLimit--;
						stackSymbol.pop_back();
						stackSymbol.push_back('A');
					}
					else{
						result += pow(2, temp1);
						temp1++;
						stackSymbol.pop_back();
						stackSymbol.push_back('A');
					}
				}
			}
			action = "归约";
		}
		step++;
		cout << step << "\t\t"
			<< symbolContent<< "\t\t"
			<< currentInputChar << "\t\t"
			<< inputContent<< "\t\t"
			<< action << endl;
	} while (!finish);

	cout << info << endl;
}


