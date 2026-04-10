using System.Xml.Linq;

namespace BagTest;

internal class Program {

	private static void Main() {

		List<int> list = [1, 2, 3];

		Enumer(list, "");
	}



	private static void Enumer(List<int> list, string str) {
		for (int i = list.Count - 1; i >= 0; i--) {
			int element = list[i];
			list.RemoveAt(i);
			Enumer(list, str + element.ToString() + " ");
			list.Add(element);

			if (list.Count == 1) Console.WriteLine(str + element.ToString());
		}
	}
}
