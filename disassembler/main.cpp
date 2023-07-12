#include <iostream>
#include <string.h>
#include <math.h>
#include <bits/stdc++.h>
#include <fstream>
using namespace std;


//bit-wise conversion
string bit_hex_to_binary_1(char s)
{
    int w = (int)s;
    w = w - 48;
    string bits;
    while (w != 0)
    {
        bits = char(w % 2 + 48) + bits;
        w = w / 2;
    }
    while (bits.length() != 4)
    {
        bits = '0' + bits;
    }
    return bits;
}
//bit-wise conversion
string bit_hex_to_binary_2(int w)
{
    string bits;
    while (w != 0)
    {
        bits = char(w % 2 + 48) + bits;
        w = w / 2;
    }
    while (bits.length() != 4)
    {
        bits = '0' + bits;
    }
    return bits;
}


//whole hex to binary conversion
string hex_to_binary(string s)
{
    string binary;
    for (size_t i = 0; i < 8; i++)
    {
        if ('0' <= s[i] && s[i] <= '9')
        {
            binary = binary + bit_hex_to_binary_1(s[i]);
        }
        else
        {
            binary = binary + bit_hex_to_binary_2((int)s[i] - 87);
        }
    }
    return binary;
}

//whole binary to decimal conversion
int bin_to_dec(string str)
{
    int w = 0;
    int i = 0;
    int l = str.length();
    while (i != l)
    {
        w = w + ((int)str[l - i - 1] - 48) * pow(2, i);
        i++;
    }
    return w;
}
int bin_to_dec_immd(string str){
 int w = 0;
    int i = 0;
    int l = str.length();
    while (i != l)
    {
        if(str[0]=='1' && i==l-1){
           w = w -((int)str[l - i - 1] - 48) * pow(2, i);
        }
        else{
        w = w + ((int)str[l - i - 1] - 48) * pow(2, i);}
        i++;
    }
    return w;
}

//R format
string R_format(string instr)
{
    string word="";
    string funt7 = instr.substr(0, 7);
    string rs2 = instr.substr(7, 5);
    string rs1 = instr.substr(12, 5);
    string funt3 = instr.substr(17, 3);
    string rd = instr.substr(20, 5);
    string a = to_string(bin_to_dec(rd));
    string b = to_string(bin_to_dec(rs1));
    string c = to_string(bin_to_dec(rs2));
    word =word+" x"+a +"," +" x"+ b+","+" x"+c;

    if (funt7 == "0000000" && funt3 == "000")
    {
        word = "add " + word;
    }
    else if (funt7 == "0100000" && funt3 == "000")
    {
        word = "sub " + word;
    }
    else if (funt7 == "0000000" && funt3 == "100")
    {
        word = "xor " + word;
    }
    else if (funt7 == "0000000" && funt3 == "110")
    {
        word = "or " + word;
    }
    else if (funt7 == "0000000" && funt3 == "111")
    {
        word = "and " + word;
    }
    else if (funt7 == "0000000" && funt3 == "001")
    {
        word = "sll " + word;
    }
    else if (funt7 == "0000000" && funt3 == "101")
    {
        word = "srl " + word;
    }
    else if (funt7 == "0100000" && funt3 == "101")
    {
        word = "sra " + word;
    }
    else if (funt7 == "0000000" && funt3 == "010")
    {
        word = "slt " + word;
    }
    else if (funt7 == "0000000" && funt3 == "011")
    {
        word = "sltu " + word;
    }
    else
    {
        word="invalid";
    }
    return word;
}

//I format
string I_format(string instr,string opcode){
   string immd=instr.substr(0,12);
   string rs1=instr.substr(12,5);
   string funt3=instr.substr(17,3);
   string rd=instr.substr(20,5);
   string cond=immd.substr(0,6);
   string immd1="";
   string word="";
    immd1=immd1+to_string(bin_to_dec_immd(immd));
   rs1=to_string(bin_to_dec(rs1));
   rd=to_string(bin_to_dec(rd));
   if(opcode=="0010011"){

    if (funt3=="000")
   {
    word="addi "+word;
   }
   else if(funt3=="100"){
    word="xori "+word;
   }
   else if(funt3=="110"){
    word="ori "+word;
   }
   else if(funt3=="111"){
    word="andi "+word;
   }
   else if(funt3=="001" && cond=="000000"){
    word="slli "+word;
   }
   else if(funt3=="101" && cond=="000000"){
    word="srli "+word;
   }
   else if(funt3=="101" && cond=="010000"){
    word="srai "+word;
    immd1=to_string(bin_to_dec(immd.substr(6,6)));
   }
   else if(funt3=="010"){
    word="slti "+word;
   }
   else if(funt3=="011"){
    word="sltiu "+word;
   }
   else{
    word="invalid";
   }
     word=word+" x"+rd+","+" x"+rs1+", "+immd1;
   }
   else if(opcode=="0000011"){
    if(funt3=="000"){
        word="lb "+word;
    }
    else if(funt3=="001"){
        word="lh "+word;
    }
    else if(funt3=="010"){
        word="lw "+word;
    }
    else if(funt3=="011"){
        word="ld "+word;
    }
    else if(funt3=="100"){
        word="lbu "+word;
    }
    else if(funt3=="101"){
        word="lhu "+word;
    }
    else if(funt3=="110"){
        word="lwu "+word;
    }
    else{
    word="invalid ";
    }
    word=word+" x"+rd+", "+immd1+"(x"+rs1+")";
   }
   else if(opcode=="1100111"){
        if(funt3=="000")word="jalr "+word;
        else
        {
            word=word+"invalid ";
        }
        word=word+" x"+rd+", "+immd1+"(x"+rs1+")";
    }
    
    
   return word;
}

//S format
string S_format(string instr){
    string immd=instr.substr(0,7)+instr.substr(20,5);
    string rs2=instr.substr(7,5);
    string rs1=instr.substr(12,5);
    string funt3=instr.substr(17,3);
   string immd1=to_string(bin_to_dec_immd(immd));
   rs1=to_string(bin_to_dec(rs1));
   rs2=to_string(bin_to_dec(rs2));
   string word="";
   if(funt3=="000"){
        word="sb "+word;
    }
    else if(funt3=="001"){
        word="sh "+word;
    }
    else if(funt3=="010"){
        word="sw "+word;
    }
    else if(funt3=="011"){
        word="sd "+word;
    }
    word=word+" x"+rs2+", "+immd1+"(x"+rs1+")";
    return word;
}

//B format
string B_format(string instr,int line_no){
    string immd=instr.substr(0,1)+instr.substr(24,1)+instr.substr(1,6)+instr.substr(20,4)+"0";
    string rs2=instr.substr(7,5);
    string rs1=instr.substr(12,5);
    string funt3=instr.substr(17,3);
    int LN=bin_to_dec_immd(immd);
   rs1=to_string(bin_to_dec(rs1));
   rs2=to_string(bin_to_dec(rs2));
   string word="";
   if(funt3=="000"){
    word="beq "+word;
   }
   else if(funt3=="001"){
     word="bne "+word;
   }
   else if(funt3=="100"){
      word="blt "+word;
   }
   else if(funt3=="101"){
      word="bge "+word;
   }
   else if(funt3=="110"){
       word="bltu "+word;
   }
   else if(funt3=="111"){
       word="bgeu "+word; 
   }
   else {
    word="invalid "+word;
   }
   word =word+"x"+ rs1+", "+"x"+rs2+", "+"L"+to_string(LN/4 + line_no);
   return word;
}

//J format
string J_format(string instr,int line_no){
      string immd=instr.substr(0,1)+instr.substr(12,8)+instr.substr(11,1)+instr.substr(1,10);
      string rd=to_string(bin_to_dec(instr.substr(20,5)));
      int LN=bin_to_dec_immd(immd+"0");
      string word;
      string label="L";
      label=label+to_string(LN/4 + line_no);
      word=word+"jal "+"x"+rd+","+label;
    return word;
}

//U format
string U_format(string instr,string immd){
    string rd=to_string(bin_to_dec(instr.substr(20,5)));
    string word="";
    word=word+"lui "+"x"+rd+","+"0x"+immd;
    return word;
}



//decoding single instruction
string decoder(string instr,string hex,int line_no)
{
    string opcode = instr.substr(25, 7);
    if (opcode == "0110011")
    {
        return R_format(instr);
    }
    else if(opcode=="0010011"||opcode=="0000011"||opcode=="1100111"){
        return I_format(instr,opcode);
    }
    else if(opcode=="0100011"){
         return S_format(instr);
    }
    else if(opcode=="1100011"){
        return B_format(instr,line_no);
    }
    else if(opcode=="1101111"){
        return J_format(instr,line_no);
    }
    else if(opcode=="0110111"){
        return U_format(instr,hex.substr(0,5));
    }
    return "";
}


//complete decoding
void decoding(string file){
      vector<string>Instructions;
      vector<int>Labels;
    ifstream input_file(file);
    string s;
    string w="";
    string opcode;
    int i=0;
    while (getline(input_file,s))
    {      
           if (s.length()!=8)
           {
            cout<<"length of the instruction is not 8,please check"<<endl;
            return;
           }
           
           w=hex_to_binary(s);
           opcode=w.substr(25, 7);
           w=decoder(w,s,i);
           if(opcode=="1100011"||opcode=="1101111"){
              Labels.push_back(stoi(w.substr(w.find("L")+1)));
           }
           Instructions.push_back(w);
           s="";
           i=i+1;
    }
    for (size_t i = 0; i < Instructions.size(); i++)
    {
        if(find(Labels.begin(),Labels.end(),i)!=Labels.end()){
           cout<<"L"<<i<<":"<<Instructions[i]<<endl;
        }
        else{
        cout<<Instructions[i]<<endl;}
    }
    
}


//main function
int main(int argc,char *argv[])
{ 
    decoding(argv[1]);
    return 0;
}