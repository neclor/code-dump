namespace Test;

internal static class Program {

	private static void Main() {

		June2019.First();
	}
}


internal static class June2019 {

	internal static void First() {
		Console.WriteLine("June 2019 (1)");

		Random rng = new();
		List<int> array = [];

		const int Value = 3;

		for (int e = 0; e < 20; e++) {
			array.Add(rng.Next(-10, 11));
		}
		array.Sort();
		Console.WriteLine(string.Join(", ", array));

		int i = 0;
		int j = array.Count - 1;
		int count = 0;

		while (i < j) {
			int diff = array[i] + array[j];

			if (diff < Value) {
				i++;

			} else if (diff > Value) {
				j--;

			} else {
				int low = array[i];
				int high = array[j];

				if (low == high) {
					int n = j - i + 1;
					count += n * (n - 1) / 2;
					break;
				}

				int lowCount = 0;
				int highCount = 0;

				for (; array[i] == low; i++) {
					lowCount++;
				}

				for (; array[j] == high; j--) {
					highCount++;
				}

				count += lowCount * highCount;
			}
		}

		Console.WriteLine(count);
	}
}
