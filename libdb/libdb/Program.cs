using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Threading.Tasks;
using Microsoft.AspNetCore;
using Microsoft.AspNetCore.Hosting;
using Microsoft.Extensions.Configuration;
using Microsoft.Extensions.Logging;


public class Program
{
    public static void Main(string[] args)
    {
        DB db = new DB();
        Console.WriteLine("ptr count = "+db.getResponseSharedPtrCount().ToString()); // 1
        var response = db.getResponseSharedPtr();
        Console.WriteLine("ptr count = "+db.getResponseSharedPtrCount().ToString()); // 2
        //response = null;
        response.Dispose();
        Console.WriteLine("ptr count = "+db.getResponseSharedPtrCount().ToString()); // 2 (!!!)   
    }
}

//Console.WriteLine("Generation = "+System.GC.GetGeneration(response).ToString());
//        System.GC.Collect();
//        GC.WaitForPendingFinalizers();
//        System.GC.Collect();
