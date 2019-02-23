using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace monitoring_smart_card_example
{
    [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Auto)]
    internal struct IoRequestCard
    {     
        private int _protocol;
        private int _pciLength;
    }
}
