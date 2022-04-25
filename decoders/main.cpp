#include <iostream>
#include <vector>
#include "ctc_beam_search_decoder.h"
#include "scorer.h"


int main() {
	freopen("test.txt", "r", stdin);
	//freopen("output.txt", "w", stdout);
	int n, m;
	std::cin >> n >> m;
	std::vector<std::vector<std::vector<double>>> logprobs(1, std::vector <std::vector<double>>(n, std::vector<double>(m)));
	for (int i = 0; i < n; ++i)
		for (int j = 0; j < m; ++j)
			std::cin >> logprobs[0][i][j];
	freopen("base.txt", "r", stdin);
	std::cin >> n;
	std::vector <int> base(n);
	for (int i = 0; i < n; ++i)
		std::cin >> base[i];

	freopen("check.txt", "r", stdin);
	std::cin >> n;
	std::vector <int> check(n);
	for (int i = 0; i < n; ++i)
		std::cin >> check[i];


	freopen("vocab.txt", "r", stdin);
	std::vector <string> vocab;
	string s;
	while(std::cin >> s)
        vocab.push_back(s);
    std::vector<std::vector<std::pair<double, string>>> results;
    KWScorer scorer = KWScorer(1.0, 1.0, 1.0, "/home/el-presidente/decoders/conf_ctc.bin", vocab, base, check);
    results = ctc_beam_search_kw_decoder_batch(logprobs, vocab, 128, 1, 1.0, 40, &scorer);
    std::cout << results.size() << ' ' << results[0].size() << std::endl;
    std::cout << results[0][0].second;
}
