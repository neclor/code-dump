namespace Test;

public interface IFoo {
	string Foo() => "interface";
}

public static class FooExtensions {
	extension(IFoo foo) {
		public string Foo() => foo.Foo();
	}
}

public class FooA : IFoo { }

public class FooB : FooA, IFoo {
	string IFoo.Foo() => "B";
}

public static class Program {
	public static void Main() {
		FooB foo = new FooB();

		Console.WriteLine(foo.Foo());
	}
}
