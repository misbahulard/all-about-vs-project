using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Tugas4_1
{
    class Fibonacci
    {
        public static void Main()
        {
            int n = 10;
            int a = 0;
            int b = 1;
            int c = 0;
    
            Console.Write(a + " " + b + " ");

            for (int i=0; i<n; i++)
            {
                c = a + b;
                a = b;
                b = c;
                Console.Write(c + " ");
            }
        }
    }
}
