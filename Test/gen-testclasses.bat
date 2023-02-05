
rem TestCommon
start /d "../cmake-build-debug/bin/" quark-header-tool.exe D:\Projects\Quark\Test\TestCommon\Sources\ D:\Projects\Quark\Test\TestCommon\Sources\DynLibTestModule.h D:\Projects\Quark\Test\TestCommon\Generated --absolute --package TestCommon --api TestCommon

rem TestDynLib
start /d "../cmake-build-debug/bin/" quark-header-tool.exe D:\Projects\Quark\Test\TestDynLib\Sources\ D:\Projects\Quark\Test\TestDynLib\Sources\MyDynLibTestModule.h D:\Projects\Quark\Test\TestDynLib\Generated --absolute --package TestDynLib --api TestDynLib

rem MainTest
start /d "../cmake-build-debug/bin/" quark-header-tool.exe D:\Projects\Quark\Test\Utility\ D:\Projects\Quark\Test\Utility\Misc\ModuleTest.h D:\Projects\Quark\Test\Generated --absolute --package Test