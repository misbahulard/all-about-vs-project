using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Tugas4_1
{
    class DeretPrima
    {
        public static void Main()
        {
            int n = 20;
            for (int i=2; i<n; i++)
            {
                bool res = true;

                if (i <= 2)
                    continue;

                for (int j=2; j<i; j++)
                {
                    if (i % j == 0)
                    {
                        res = false;
                        break;
                    }
                }
                if (res)
                    Console.Write(i + " ");
            }
        }
    }
}
