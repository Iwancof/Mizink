using System;
using System.Linq;
using System.IO;
using System.Text;

class MainClass {
  static void Main() {
    StreamReader sr = new StreamReader("constants.vls", Encoding.GetEncoding("Shift_JIS"));
    string str = sr.ReadToEnd();
    sr.Close();

    StreamWriter sw_NASM = new StreamWriter("define.s", false, Encoding.GetEncoding("Shift_JIS"));
    StreamWriter sw_C = new StreamWriter("define.c", false, Encoding.GetEncoding("Shift_JIS"));
    
    str = str.Replace(" ","");
    str = str.Replace("\t","");

    foreach(var e in str.Split('\n')) {
      	if(e.Length == 0) continue;
    	if(e[0] == '#') continue;
	sw_NASM.Write(for_NASM(e));
	sw_C.Write(for_C(e));
    }

    sw_NASM.Close();
    sw_C.Close();
  }

  static string for_NASM(string str) {
    var ret = str.Split(':');
    if(ret.Length != 2)
      return "";
    string name = ret[0];
    string value = ret[1];
    return String.Format("{0}	equ	{1}\n", name, value);
  }
  static string for_C(string str) {
    str = str.Replace("\n", "");
    str = str.Replace("\r", "");
    var ret = str.Split(':');
    if(ret.Length != 2)
      return "\n";
    string name = ret[0];
    string value = ret[1];
    return String.Format("#define	{0}	({1})\n", name, value);
  }
}



