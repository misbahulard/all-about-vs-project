using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Tugas4_1
{
    class While2
    {
        public static void Main()
        {
            int i = 0;
            while (true)
            {
                Console.WriteLine(i);
                i++;
                if (i > 10)
                    break;
            }
        }
    }
}
