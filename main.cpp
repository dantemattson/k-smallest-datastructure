#include <iostream>
#include <chrono>
#include <vector>
#include <set>

class KSmallest {
public:
	std::set<int> overflow;
	std::set<int> kSmallest;
	int elementsWanted;
	int maxInKSmallest = 0;

	// O(1)
	KSmallest(int wanted) {
		elementsWanted = wanted;
	}

	//  N log(size + N)
	int popSmallest() {
		int s = getSmallest();
		remove(s);
		return s;
	}
	
	// O(1)
	int getSmallest() {
		if (kSmallest.empty()) {
			throw "Get on empty\n";
		}
		
		return *kSmallest.begin();
	}

	// O(1)
	void updateIfHigher(int w) {
		if (w > maxInKSmallest) {
			maxInKSmallest = w;
		}
		return;
	}

	// N log(size + N), linear if elements sorted + O(k)
	void push(int what) {

		// Constant and happens once
		if (kSmallest.size() == 0) {
			// Nlog(size+N)
			kSmallest.insert(what);
			maxInKSmallest = what;
		}
		// Happens k times, much less than n
		else if (kSmallest.size() < elementsWanted) {
			// Nlog(size+N)
			kSmallest.insert(what);
			updateIfHigher(what);
		}
		// N log(size + N)
		else {
			if (what < maxInKSmallest) {
				// Nlog(size+N)
				kSmallest.insert(what);

				if (kSmallest.size() > elementsWanted) {
					int rb = *kSmallest.rbegin();
					// Nlog(size+N)
					overflow.insert(rb);
					// O(elementsWanted)
					kSmallest.erase(rb);
				}

				updateIfHigher(what);

			}
			else {
				// Nlog(size+N)
				overflow.insert(what);
			}

		}

	}

	// N log(size + N)
	void remove(int what) {

		if (kSmallest.size() == 0) {
			return;
		}
		else {

			if (what <= maxInKSmallest) {

				// O(n-k), amort O(1)
				kSmallest.erase(what);

				// "pop" to maintain as many ints as possible
				// Is a while loop but only really executes once
				while (kSmallest.size() < elementsWanted) {
					if (overflow.size() > 0) {
						auto bg = overflow.begin();
						// N log(k + N)
						kSmallest.insert(*bg);
						// O(n-k), amort O(1)
						overflow.erase(bg);
					}
				}

				maxInKSmallest = *kSmallest.rbegin();

			}
			else {
				// O(n-k), w is size of overflow, amort O(1)
				overflow.erase(what);
			}

		}
		
		return;
	}
	
	// Show the structure
	void toString() {

		std::cout << "smallests:\n";
		for (int i : kSmallest) {
			std::cout << i << ", ";
		}
		std::cout << "\noverflow: "<< overflow.size() <<"\n";
		// To verify the contents of overflow manually, uncomment below
		/*auto i = overflow.begin();
		while (i != overflow.end()) {
			cout << *i << ", ";
			i++;
		}
		cout << "\n";*/
		
		return;
	}

};

int main() {

	// stores the k smallest ints
	KSmallest ksmallestTest = KSmallest(25);

	std::cout << "How many integers do you want to insert?\n";

	int mill = 0;
	std::cin >> mill;

	std::vector<int> v;
	std::vector<int> removeLater;
	v.reserve(mill);
	removeLater.reserve(mill/2);

	// add elements
	for (int i = mill; i > 0; i--) {
		v.push_back(i);
	}

	// randomise v
	int size = v.size();
	for (int i = 0; i < size - 1; i++) {
		// merge of swap and...
		int j = i + rand() % (size - i);
		std::swap(v[i], v[j]);
		// ... the rolling of random chance for integers to be removed again later
		if (rand() % 2) {
			removeLater.push_back(i);
		}
	}
	
	// randomise removeLater
	size = removeLater.size();
	for (int i = 0; i < size - 1; i++) {
		int j = i + rand() % (size - i);
		std::swap(v[i], v[j]);
	}
	
	// Testing removing of integers known not to be in the set
	//removeLater.push_back(-1);
	//removeLater.push_back(-2);
	
	auto start = std::chrono::high_resolution_clock::now();
	
	for (int i : v) {
		ksmallestTest.push(i);
	}
	
	for (int i : removeLater) {
		ksmallestTest.remove(i);
	}

	/*
	Some simple manual testing
	
	ksmallestTest.remove(11);
	ksmallestTest.push(11);
	ksmallestTest.push(1);
	ksmallestTest.push(2);
	
	ksmallestTest.push(5);
	ksmallestTest.push(3);
	ksmallestTest.push(2);
	ksmallestTest.push(9);
	ksmallestTest.remove(9);
	ksmallestTest.push(-1);
	*/

	std::cout << "Program total: " << std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - start).count() << "ms\n";

	ksmallestTest.toString();

	return 0;
}