#include <iostream>
#include <vector>
#include <algorithm>

struct Word {
  int id;
  int position;
};

void testcase() {
  int n; std::cin >> n;
  std::vector<int> word_occurences(n);
  for (int i = 0; i < n; ++i) {
    std::cin >> word_occurences[i];
  }
  std::vector<Word> words;
  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < word_occurences[i]; ++j) {
      int p; std::cin >> p;
      words.push_back(Word{i, p});
    }
  }
  std::sort(words.begin(), words.end(), [](const Word& l, const Word& r) 
      { return l.position < r.position; });

  int l = 0, r = 0;
  int distinct_word_counter = 0;
  std::vector<int> count_vector(n, 0);
  int best = std::numeric_limits<int>::max();
  while (true) {
    if (distinct_word_counter < n) {
      Word new_word = words[++r - 1];
      if (++count_vector[new_word.id] == 1)
        ++distinct_word_counter;
    } else {
      Word removed_word = words[l++];
      if (--count_vector[removed_word.id] == 0)
        --distinct_word_counter;
    }
    if (distinct_word_counter == n) {
      int value = words[r - 1].position - words[l].position + 1;
      if (value < best) best = value;
    }
    
    if (r == words.size() && distinct_word_counter < n)
      break;
  }
  std::cout << best << "\n";
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int t; std::cin >> t;
  for (int i = 0; i < t; ++i) {
    testcase();
  }
}

