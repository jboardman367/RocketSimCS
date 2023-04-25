## GoalSoreCallback
```csharp
namespace Std
{
    namespace Function
    {
        [StructLayout(LayoutKind.Sequential, Size = 64, Pack = 8)]
        public unsafe partial struct __Internal
        {
            internal global::Std.FuncClass.Storage.__Internal _Mystorage;
        }
    }

    namespace FuncClass
    {
        [StructLayout(LayoutKind.Sequential, Size = 64, Pack = 8)]
        public unsafe partial struct __Internal
        {
            internal global::Std.FuncClass.Storage.__Internal _Mystorage;
        }

        namespace Storage
        {
            [StructLayout(LayoutKind.Explicit, Size = 64, Pack = 8)]
            public unsafe partial struct __Internal
            {
                [FieldOffset(0)]
                internal double _Dummy1;

                [FieldOffset(0)]
                internal fixed sbyte _Dummy2[56];

                [FieldOffset(0)]
                internal fixed long _Ptrs[8];
            }
        }

    }
}
```
## public CarState()
remove ctor call

same with Vec()

just remember to assign initial values yourself