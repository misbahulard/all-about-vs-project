using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Tugas4_1
{
    class For2
    {
        public static void Main()
        {
            int i = 0;
            for (; ; )
            {
                if (i < 10)
                {
                    Console.WriteLine(i);
                    i++;
                }
                else
                    break;
            }
        }
    }
}
