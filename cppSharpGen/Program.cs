// See https://aka.ms/new-console-template for more information
using CppSharp;
using CppSharp.AST;
using CppSharp.Generators;

ConsoleDriver.Run(new RocketSimLibrary());
Console.WriteLine("Generated C# library");

class RocketSimLibrary : ILibrary
{
    public void Postprocess(Driver driver, ASTContext ctx)
    {
        // No action
    }

    public void Preprocess(Driver driver, ASTContext ctx)
    {
        // No action
    }

    public void Setup(Driver driver)
    {
        // Use a C# generator
        var options = driver.Options;
        options.GeneratorKind = GeneratorKind.CSharp;
        options.OutputDir = "..\\RocketSimCS";
        driver.ParserOptions.LanguageVersion = CppSharp.Parser.LanguageVersion.CPP20;

        var module = options.AddModule("RocketSim");
        // Add include dirs (where the headers are)

        // Needed for RocketSim::Init
        module.IncludeDirs.Add("..\\src");
        module.Headers.Add("RocketSim.h");

        // Needed for Arena::Create
        module.IncludeDirs.Add("..\\src\\Sim\\Arena");
        module.Headers.Add("Arena.h");
        module.IncludeDirs.Add("..\\src\\Sim\\BoostPad\\BoostPadGrid");
        module.Headers.Add("BoostPadGrid.h");
        module.IncludeDirs.Add("..\\src\\Sim\\SuspensionCollisionGrid");
        module.Headers.Add("SuspensionCollisionGrid.h");

        // Needed for Arena.AddCar
        module.IncludeDirs.Add("..\\src\\Sim\\Car");
        module.Headers.Add("Car.h");
        module.IncludeDirs.Add("..\\src\\Sim\\Car\\CarConfig");
        module.Headers.Add("CarConfig.h");
        module.IncludeDirs.Add("..\\src\\Sim");
        module.Headers.Add("CarControls.h");
        module.IncludeDirs.Add("..\\src\\Math\\MathTypes");
        module.Headers.Add("MathTypes.h");

        // Ball
        module.IncludeDirs.Add("..\\src\\Sim\\Ball");
        module.Headers.Add("Ball.h");

        // Add library dirs (where the .lib(s) are)
        module.LibraryDirs.Add("..\\out\\build\\x64-Release");
        module.Libraries.Add("RocketSim.dll");
    }

    public void SetupPasses(Driver driver)
    {
        // No action
    }
}
